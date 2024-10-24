#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>  // For numeric_limits
using namespace std;

const int ALPHABET_SIZE = 26;

class TrieNode {
public:
    TrieNode* children[ALPHABET_SIZE];
    bool isEndOfWord;

    TrieNode() {
        isEndOfWord = false;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            children[i] = nullptr;
        }
    }
};

class Trie {
private:
    TrieNode* root;

    // Utility function to convert character to index
    int charToIndex(char c) {
        return c - 'a';
    }

public:
    Trie() {
        root = new TrieNode();
    }

    // Insert a guest's name into the Trie
    void insert(string key) {
        TrieNode* node = root;
        for (int i = 0; i < key.length(); i++) {
            int index = charToIndex(tolower(key[i]));
            if (!node->children[index])
                node->children[index] = new TrieNode();
            node = node->children[index];
        }
        node->isEndOfWord = true;
    }

    // Search for a guest's name in the Trie
    bool search(string key) {
        TrieNode* node = root;
        for (int i = 0; i < key.length(); i++) {
            int index = charToIndex(tolower(key[i]));
            if (!node->children[index])
                return false;
            node = node->children[index];
        }
        return (node != nullptr && node->isEndOfWord);
    }

    // Delete a guest from the Trie
    bool remove(TrieNode* node, string key, int depth = 0) {
        if (!node)
            return false;

        // If last character of key is being processed
        if (depth == key.size()) {
            if (!node->isEndOfWord)
                return false;

            node->isEndOfWord = false;
            return isEmpty(node);
        }

        int index = charToIndex(tolower(key[depth]));
        if (remove(node->children[index], key, depth + 1)) {
            delete node->children[index];
            node->children[index] = nullptr;

            return !node->isEndOfWord && isEmpty(node);
        }

        return false;
    }

    void remove(string key) {
        remove(root, key);
    }

    // Check if TrieNode has no children
    bool isEmpty(TrieNode* node) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (node->children[i])
                return false;
        }
        return true;
    }

    // Load guest list from a file
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        string guest;
        while (getline(file, guest)) {
            insert(guest);
        }
        file.close();
    }

    // Save guest list to a file
    void saveToFile(const string& filename, TrieNode* node, string prefix) {
        if (node->isEndOfWord)
            ofstream(filename, ios::app) << prefix << endl;

        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (node->children[i]) {
                char nextChar = 'a' + i;
                saveToFile(filename, node->children[i], prefix + nextChar);
            }
        }
    }

    void saveToFile(const string& filename) {
        ofstream file(filename);
        file.close();  // Clear the file
        saveToFile(filename, root, "");
    }

    // Check if the guest list is empty
    bool isEmpty() {
        return isEmpty(root);
    }
};

// Main menu for the program
void menu(Trie& guestList) {
    int choice;
    string guest;
    const string filename = "guest_list.txt";

    guestList.loadFromFile(filename);

    do {
        cout << "\n=== Nightclub Guest List ===\n";
        cout << "1. Insert Guest\n";
        cout << "2. Search Guest\n";
        cout << "3. Delete Guest\n";
        cout << "4. Check if Guest List is Empty\n";
        cout << "5. Exit and Save\n";
        cout << "Enter your choice (1-5): ";

        // Input validation to ensure the choice is a valid number between 1 and 5
        while (true) {
            cin >> choice;

            // Check if the input is a valid integer and within the range of 1 to 5
            if (cin.fail() || choice < 1 || choice > 5) {
                cin.clear();  // Clear the error flag on cin
                cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore invalid input
                cout << "Invalid input. Please enter a number between 1 and 5: ";
            }
            else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear the input buffer
                break;  // Valid input, exit the loop
            }
        }

        switch (choice) {
        case 1:
            cout << "Enter guest name: ";
            getline(cin, guest);  // Get full name including spaces
            guestList.insert(guest);
            cout << guest << " added to the guest list.\n";
            break;
        case 2:
            cout << "Enter guest name: ";
            getline(cin, guest);  // Get full name including spaces
            if (guestList.search(guest))
                cout << guest << " is on the guest list.\n";
            else
                cout << guest << " is NOT on the guest list.\n";
            break;
        case 3:
            cout << "Enter guest name: ";
            getline(cin, guest);  // Get full name including spaces
            guestList.remove(guest);
            cout << guest << " removed from the guest list.\n";
            break;
        case 4:
            if (guestList.isEmpty())
                cout << "The guest list is empty.\n";
            else
                cout << "The guest list is NOT empty.\n";
            break;
        case 5:
            guestList.saveToFile(filename);
            cout << "Guest list saved.\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);
}

int main() {
    Trie guestList;
    menu(guestList);
    return 0;
}
