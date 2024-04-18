#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

struct TrieNode {
    int wordCount;
    struct TrieNode *children[ALPHABET_SIZE];
};

struct Trie {
    struct TrieNode *root;
};

struct Trie *createTrie() {
    struct Trie *pTrie = (struct Trie *)malloc(sizeof(struct Trie));
    pTrie->root = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    pTrie->root->wordCount = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        pTrie->root->children[i] = NULL;
    }
    return pTrie;
}

void insert(struct Trie *pTrie, char *word) {
    struct TrieNode *current = pTrie->root;
    while (*word) {
        int index = *word - 'a';
        if (!current->children[index]) {
            current->children[index] = (struct TrieNode *)malloc(sizeof(struct TrieNode));
            current->children[index]->wordCount = 0;
            for (int i = 0; i < ALPHABET_SIZE; i++) {
                current->children[index]->children[i] = NULL;
            }
        }
        current = current->children[index];
        word++;
    }
    current->wordCount++;
}

int numberOfOccurances(struct Trie *pTrie, char *word) {
    struct TrieNode *current = pTrie->root;
    while (*word) {
        int index = *word - 'a';
        if (!current->children[index]) {
            return 0;
        }
        current = current->children[index];
        word++;
    }
    return current->wordCount;
}

struct Trie *deallocateTrie(struct Trie *pTrie) {
    // Recursive helper function to free nodes
    void freeNode(struct TrieNode *node) {
        if (node == NULL) return;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (node->children[i] != NULL) {
                freeNode(node->children[i]);
            }
        }
        free(node);
    }

    freeNode(pTrie->root);
    free(pTrie);
    return NULL;
}

int readDictionary(char *filename, char **pInWords) {
    FILE *file = fopen(filename, "r");
    int numWords = 0;
    if (file == NULL) {
        printf("Error opening file.\n");
        return 0;
    }

    char buffer[100];
    fgets(buffer, sizeof(buffer), file); // Read and ignore the count line
    while (fgets(buffer, sizeof(buffer), file) && numWords < 256) {
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character
        pInWords[numWords++] = strdup(buffer);
    }
    fclose(file);
    return numWords;
}

int main(void) {
    char *inWords[256];

    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }


    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++) {
        insert(pTrie, inWords[i]);
    }

    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }

    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL) {
        printf("There is an error in this program\n");
    }

    // Free allocated memory for words
    for (int i = 0; i < numWords; i++) {
        free(inWords[i]);
    }

    return 0;
}