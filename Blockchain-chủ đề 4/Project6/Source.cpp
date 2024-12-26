#include <iostream>
#include <string>
#include <sstream>
#include <functional>
#include <ctime>
#include <map>
using namespace std;

struct Block {
    int index;
    string data;
    string previousHash;
    string hash;
    time_t timestamp;
    Block* next;

    Block(int idx, const string& d, const string& prevHash)
        : index(idx), data(d), previousHash(prevHash), next(nullptr) {
        timestamp = time(nullptr);
        hash = calculateHash();
    }

    string calculateHash() const {
        stringstream ss;
        ss << index << data << previousHash << timestamp;
        return hashFunction(ss.str());
    }

    static string hashFunction(const string& input) {
        return to_string(std::hash<string>{}(input));
    }
};

class Blockchain {
private:
    Block* head;
    Block* tail;

public:
    Blockchain() {
        head = new Block(0, "Genesis Block", "0");
        tail = head;
    }

    ~Blockchain() {
        while (head) {
            Block* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void addBlock(const string& data) {
        Block* newBlock = new Block(tail->index + 1, data, tail->hash);
        tail->next = newBlock;
        tail = newBlock;
    }

    bool isChainValid() const {
        Block* current = head->next;
        Block* previous = head;

        while (current) {
            if (current->hash != current->calculateHash()) {
                return false;
            }
            if (current->previousHash != previous->hash) {
                return false;
            }

            previous = current;
            current = current->next;
        }
        return true;
    }

    void printChain() const {
        Block* current = head;
        while (current) {
            char buffer[26];
            ctime_s(buffer, sizeof(buffer), &current->timestamp);
            cout << "Index: " << current->index << "\n"
                << "Data: " << current->data << "\n"
                << "Previous Hash: " << current->previousHash << "\n"
                << "Hash: " << current->hash << "\n"
                << "Timestamp: " << buffer << "\n";
            current = current->next;
        }
    }

    void addBlockFromInput() {
        string data;
        cout << "Enter data for the new block: ";
        getline(cin, data);
        addBlock(data);
    }

    void analyzeChain() const {
        int totalBlocks = 0;
        map<string, int> dataDistribution;
        double totalTimeDiff = 0;
        int timeIntervals = 0;

        Block* current = head;
        Block* previous = nullptr;

        while (current) {
            totalBlocks++;
            dataDistribution[current->data]++;

            if (previous) {
                totalTimeDiff += difftime(current->timestamp, previous->timestamp);
                timeIntervals++;
            }

            previous = current;
            current = current->next;
        }

        cout << "Blockchain Analysis:\n";
        cout << "Total blocks: " << totalBlocks << "\n";

        cout << "Data distribution:\n";
        for (const auto& entry : dataDistribution) {
            cout << "  Data: " << entry.first << " - Count: " << entry.second << "\n";
        }

    }

    void searchBlockByData(const string& query) const {
        Block* current = head;
        bool found = false;
        while (current) {
            if (current->data == query) {
                char buffer[26];
                ctime_s(buffer, sizeof(buffer), &current->timestamp);

                cout << "Block found:\n";
                cout << "Index: " << current->index << "\n"
                    << "Data: " << current->data << "\n"
                    << "Previous Hash: " << current->previousHash << "\n"
                    << "Hash: " << current->hash << "\n"
                    << "Timestamp: " << buffer << "\n";
                found = true;
                break;
            }
            current = current->next;
        }
        if (!found) {
            cout << "No block found with the data: " << query << "\n";
        }
    }
};

int main() {
    Blockchain blockchain;
    int choice;

    do {
        cout << "Blockchain Menu:\n";
        cout << "1. Add a new block\n";
        cout << "2. Display the blockchain\n";
        cout << "3. Verify blockchain integrity\n";
        cout << "4. Analyze the blockchain\n";
        cout << "5. Search for a block by data\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            blockchain.addBlockFromInput();
            break;
        case 2:
            blockchain.printChain();
            break;
        case 3:
            cout << "Is blockchain valid? " << (blockchain.isChainValid() ? "Yes" : "No") << endl;
            break;
        case 4:
            blockchain.analyzeChain();
            break;
        case 5: {
            string query;
            cout << "Enter the data to search for: ";
            getline(cin, query);
            blockchain.searchBlockByData(query);
            break;
        }
        case 6:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}
