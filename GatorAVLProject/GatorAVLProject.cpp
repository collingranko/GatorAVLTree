// GatorAVLProject.cpp : This file contains the 'main' function.

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// ===== File Includes and Using's ===== //
// Includes
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

// List of using's because namespace std was causing errors for me
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::istringstream;
using std::max;

// ===== avlTree class and nested Node Struct ===== //
class avlTree
{
	struct Node
	{
		// Straightforward variables for the node
		// Only thing to note is the int version and separate string version of the id
		// For comparing IDs easily while still having easy output (specifically leading 0s case)
		string gatorName;
		int gatorID;
		string gatorIDString;
		Node* left;
		Node* right;
		int height;
		Node()
		{
			// default constructor
			gatorName = "";
			gatorID = 0;
			gatorIDString = "";
			left = nullptr;
			right = nullptr;
			height = 1;
		}
		Node(string name, int id, string idStr)
		{
			// overloaded constructor
			gatorName = name;
			gatorID = id;
			gatorIDString = idStr;
			left = nullptr;
			right = nullptr;
			height = 1;
		}
	};

	// ===== avlTree Variables ===== // - only variable for my tree object is the root of it
	Node* root = nullptr;

	// ===== Some Stepik Functions ===== //
	// isAVL function from stepik 5.2.1
	bool isAVL(Node* root)
	{
		// Thought this would come in handy but ultimately might not use it
		// Returns a T/F value if the tree is an AVL Tree or not
		if (root == nullptr)
		{
			return true;
		}
		setHeight(root);
		int leftTree = 0;
		if (root->left != nullptr)
		{
			leftTree = root->left->height;
		}
		int rightTree = 0;
		if (root->right != nullptr)
		{
			rightTree = root->right->height;
		}
		int balanceFactor = leftTree - rightTree;
		if (balanceFactor >= -1 && balanceFactor <= 1)
		{
			if (isAVL(root->left) && isAVL(root->right))
			{
				return true;
			}
		}
		return false;
	}

	// rotate functions from my stepik 5.1.1 - no comments pretty straightforward
	Node* leftRotate(Node* node)
	{
		Node* newRoot = node->right;
		node->right = node->right->left;
		newRoot->left = node;
		setHeight(newRoot);
		return newRoot;
	}
	Node* rightRotate(Node* node)
	{
		Node* newRoot = node->left;
		node->left = node->left->right;
		newRoot->right = node;
		setHeight(newRoot);
		return newRoot;
	}
	Node* leftRightRotate(Node* node)
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}
	Node* rightLeftRotate(Node* node)
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	// ===== Helper Functions ===== // - not required but functions that helped me and get called in various scenarios
	int getBF(Node* node)
	{
		// Returns the balance factor of the node inputted = Left subtree - Right subtree
		// Leverages nullptrs to prevent error
		// An empty subtree is 0 and a subtree of 1 node is 1
		setHeight(node);
		if (node == nullptr)
		{
			return 0;
		}
		int leftTree = 0;
		if (node->left != nullptr)
		{
			leftTree = node->left->height;
		}
		int rightTree = 0;
		if (node->right != nullptr)
		{
			rightTree = node->right->height;
		}
		return leftTree - rightTree;
	}
	int setHeight(Node* currentNode)
	{
		// Helper function used throughout my code to update the height of inputted node and ones below it
		// Iterate through all nodes setting the height variable to its proper number
		// Tree with one node has a height of 1 and empty tree is 0 for clarification
		if (currentNode == nullptr)
		{
			return 0;
		}
		else
		{
			currentNode->height = 1 + max(setHeight(currentNode->left), setHeight(currentNode->right));
			return currentNode->height;
		}
	}
	Node* balanceTree(Node* node, int ID)
	{
		// Several layering functions for balancing and inserting as this was the harderst part for me to code

		// Using weight to find balance factor and call correct rotations- from TA session / stepik

		// First get balance factor of the node - calling getBF will set the height for this node and all below it
		int balanceFactor = getBF(node);

		// Use the ID of the node inserted to determine where the newly gained "weight" will be
		// Same as finding the balance factor of the child of this node

		// Uses the balance factor of this node and the determined weight of the child node through ID to call a rotation if necessary
		if (balanceFactor > 1)
		{
			if (ID < node->left->gatorID)
			{
				return rightRotate(node);
			}
			else if (ID > node->left->gatorID)
			{
				return leftRightRotate(node);
			}
		}
		if (balanceFactor < -1)
		{
			if (ID > node->right->gatorID)
			{
				return leftRotate(node);
			}
			else if (ID < node->right->gatorID)
			{
				return rightLeftRotate(node);
			}
		}
		// Always returns a node even if unchanged to allow recursion and keep track of root pointer
		return node;
	}

public:
	// Destructor for all allocated memory
	~avlTree()
	{
		// destructor just in case
		// Note: I have a helper function that will store all nodes in the tree onto a single vector
		// not space efficient but can get the job done here aswell

		vector<Node*> nodesToBeDeleted;
		nodesToBeDeleted = removeInorderNHelper(0, root, nodesToBeDeleted);

		// Vector of all nodes in the tree in the order of inorder
		// Now just loop through and delete

		for (unsigned int i = 0; i < nodesToBeDeleted.size(); i++)
		{
			delete nodesToBeDeleted[i];
		}
	}

	// ===== Multi-functional Helpers =====//
	Node* getAncestor(Node* child)
	{
		// helper function for fixing imbalances
		// takes in parent of inserted node and returns grandparent for balancing
		Node* tempNode = root;
		while (tempNode != nullptr)
		{
			if (child->gatorID > tempNode->gatorID)
			{
				if (child->gatorID == tempNode->right->gatorID)
				{
					return tempNode;
				}
				tempNode = tempNode->right;
			}
			else if (child->gatorID < tempNode->gatorID)
			{
				if (child->gatorID == tempNode->left->gatorID)
				{
					return tempNode;
				}
				tempNode = tempNode->left;
			}
			else
			{
				// No parent pointer
				return nullptr;
			}
		}
		return tempNode;
	}

	// ===== Insertion (Helpers included) ===== //
	void inser(Node* node, string NAME, int ID, string IDStr)
	{
		// Inser is just a call function to keep the main more clean
		root = insert(node, NAME, ID, IDStr);
	}
	Node* insert(Node* node, string NAME, int ID, string IDStr)
	{
		// Recursively travels to the proper location for a new node to be inserted
		// Several cases:

		/* Case 1: no nodes in tree OR reached insertion spot
			in this case I return a new Node for its parent to grab or the root is set at the tree's first node */
		if (node == nullptr)
		{
			cout << "successful" << endl;
			return(new Node(NAME, ID, IDStr));
		}
		/* Case 2: duplicate node
			if the ID in the tree already exists, print unsuccessful and end this recursion path */
		else if (ID == node->gatorID)
		{
			cout << "unsuccessful" << endl;
			return node;
		}
		/* Case 3: traversal
			using properties of this tree, I know to traverse left or right based on the ID number. Recursively calls itself */
		else if (ID < node->gatorID)
		{
			node->left = insert(node->left, NAME, ID, IDStr);
		}
		else if (ID > node->gatorID)
		{
			node->right = insert(node->right, NAME, ID, IDStr);
		}

		// After node is inserted I call my helper function to balance the tree
		return balanceTree(node, ID);
		}

	// ===== Searches (Helpers included) ===== //
	void searchID(int ID)
	{
		// Given an ID, search for the name
		// Several cases:

		/* If empty tree, ID is not in the tree. Prints unsuccessful */
		if (root == nullptr)
		{
			// unsuccessful case - unpopulated tree
			cout << "unsuccessful" << endl;
		}
		/* For a populated tree and using the tree properties, I can traverse through to where the node should
			be. If it's not there, then the node is no longer or never was in the tree. However, if it is found, loop
			is broken and name is printed. */
		else
		{
			// loop until no nodes left to search or until id is found
			Node* tempNode = root;
			bool idFound = false;
			while (tempNode != nullptr)
			{
				if (ID < tempNode->gatorID)
				{
					tempNode = tempNode->left;
				}
				else if (ID > tempNode->gatorID)
				{
					tempNode = tempNode->right;
				}
				else if (ID == tempNode->gatorID)
				{
					// successful case - end loop
					cout << tempNode->gatorName << endl;
					idFound = true;
					break;
				}
			}
			// If the id was not found print unsuccessful
			if (!idFound)
			{
				cout << "unsuccessful" << endl;
			}
		}
	}
	void searchNAME(string NAME)
	{
		// Searches for all occurences of a specified name in the tree
		// Several cases:

		/* Empty tree means unsuccessful search */
		if (root == nullptr)
		{
			// unsuccessful case - unpopulated tree
			cout << "unsuccessful" << endl;
		}
		/* For simplicity, calls a recursive helper function to print any and all occurrences of the name
			in a pre-order traversal order. Returns the number of occurrences found. If 0, unsuccessful search */
		else
		{
			// Helper recursive function to count number of matches found if any
			int matchesFound = recursiveNameHelper(root, NAME, 0);
			if (matchesFound == 0)
			{
				cout << "unsuccessful" << endl;
			}
		}
	}
	int recursiveNameHelper(Node* currentNode, string soughtName, int sum)
	{
		// Helper functions to print out the searched for name in a pre-order traversal order
		if (currentNode->gatorName == soughtName)
		{
			// one match found
			cout << currentNode->gatorIDString << endl;
			sum += 1;
		}
		else
		{
			// check children recursively
			if (currentNode->left != nullptr)
			{
				sum += recursiveNameHelper(currentNode->left, soughtName, sum);
			}
			if (currentNode->right != nullptr)
			{
				sum += recursiveNameHelper(currentNode->right, soughtName, sum);
			}
		}
		return sum;
	}

	// ===== Printing (Traversals) ===== //
	void printInorder(Node* node)
	{
		// Print tree in a inorder manner - LRoR
		// Leveraged recursion and pointer knowledge to splice output with comma when I know another name is being printed
		if (node == nullptr)
		{
			// do nothing
		}
		else
		{
			if (node->left != nullptr)
			{
				printInorder(node->left);
				cout << ", ";

			}
			cout << node->gatorName;
			if (node->right != nullptr)
			{
				cout << ", ";
				printInorder(node->right);
			}
		}
	}
	void printPreorder(Node* node)
	{
		// Print tree in a preorder manner - RoLR
		// I leveraged knowing the root pointer is seperate from the parameter to
		// comma splice my output
		if (node == nullptr)
		{
			return;
		}
		if (root == node)
		{
			cout << node->gatorName;
			printPreorder(node->left);
			printPreorder(node->right);
		}
		else
		{
			cout << ", " << node->gatorName;
			printPreorder(node->left);
			printPreorder(node->right);
		}
	}
	void printPostorder(Node* node)
	{
		// Print tree in a preorder manner - RLRo
		// Leveraged recursion and pointer knowledge to splice output with comma when I know another name is being printed
		if (node == nullptr)
		{
			return;
		}
		else
		{
			printPostorder(node->left);
			if (node->left != nullptr)
			{
				cout << ", ";
			}
			printPostorder(node->right);
			if (node->right != nullptr)
			{
				cout << ", ";
			}
			cout << node->gatorName;
		}
	}
	void printLevelCount()
	{
		// Prints the level count which is equivalent to height of the root
		// 0 if tree is empty
		if (root == nullptr)
		{
			cout << 0 << endl;
		}
		else
		{
			setHeight(root);
			cout << root->height << endl;
		}
	}

	// ===== Accessors ===== // 
	Node* getHead()
	{
		// Accessor so root cannot be changed in main / gives root access to main for function calls
		return root;
	}

	// ===== Node Removal ===== //
	void removeID(int ID, Node* node, Node* parent)
	{
		// Removes specified ID in the tree
		// Factors in multiple cases where the root may be changed
		// The algorithm changes for each number of children a node has
		// Includes a search snippet of code to traverse aswell

		// If empty tree print unsuccessful
		if (node == nullptr)
		{
			cout << "unsuccessful" << endl;
			return;
		}

		if (parent == nullptr)
		{
			parent = getAncestor(node);
		}

		// Special case of root deletion
		if (ID == root->gatorID)
		{
			Node* tempNode = root;
			if (root->left == nullptr && root->right == nullptr)
			{
				// Case 1: Root to be deleted has no children
				// Empty tree
				delete root;
				root = nullptr;
			}
			else if (root->left != nullptr && root->right != nullptr)
			{
				// Case 2: Root to be deleted has 2 children
				// Finding successor
				parent = root;
				Node* successorNode = root->right;
				while (successorNode->left != nullptr)
				{
					parent = successorNode;
					successorNode = successorNode->left;
				}
				successorNode->left = root->left;
				parent->left = nullptr;
				successorNode->right = root->right->right;
				delete root;
				root = successorNode;
			}
			else if (root->left != nullptr)
			{
				// Case 3: Root to be deleted has one child
				Node* tempNode = root;
				delete tempNode;
				root = root->left;
			}
			else if (node->right != nullptr)
			{
				// Case 3: Root to be deleted has one child (cont.)
				Node* tempNode = root;
				delete tempNode;
				root = root->right;
			}
			cout << "successful" << endl;
			setHeight(root);

			return;
		}

		// Search for node that will be deleted
		if (ID < node->gatorID)
		{
			removeID(ID, node->left, node);
		}
		else if (ID > node->gatorID)
		{
			removeID(ID, node->right, node);
		}
		else if (ID == node->gatorID)
		{
			// 3 Cases for deletion
			if (node->left == nullptr && node->right == nullptr)
			{
				// Case 1: Node to be deleted has no children
				if (parent->left == node)
				{
					delete node;
					parent->left = nullptr;
				}
				else if (parent->right == node)
				{
					delete node;
					parent->right = nullptr;
				}
			}
			else if (node->left != nullptr && node->right != nullptr)
			{
				// Case 2: Node to be deleted has 2 children
				// Finding successor
				Node* successorNode = node->right;
				while (successorNode->left != nullptr)
				{
					successorNode = successorNode->left;
				}
				successorNode->left = node->left;
				if (parent->right == node)
				{
					parent->right = successorNode;
				}
				else if (parent->left == node)
				{
					parent->left = successorNode;
				}
				delete node;
			}
			else if (node->left != nullptr)
			{
				// Case 3: Node to be deleted has one child
				if (parent->left == node)
				{
					parent->left = node->left;
					delete node;
				}
				else if (parent->right == node)
				{
					parent->right = node->left;
					delete node;
				}
			}
			else if (node->right != nullptr)
			{
				// Case 3: Node to be deleted has one child (cont.)
				if (parent->left == node)
				{
					parent->left = node->right;
					delete node;
				}
				else if (parent->right == node)
				{
					parent->right = node->left;
					delete node;
				}
			}
			cout << "successful" << endl;
			setHeight(root);
		}
		else
		{
			// Node is not in the tree
			cout << "unsuccessful" << endl;
		}
	}
	void removeInorderN(int nTh)
	{
		// Utilizes a vector, a helper function, and my previous remove function to remove the Nth node in an inorder traversal
		// Vector to ultimately find the Nth node
		// helper for recursion and a more simple algorithm to read
		// previous function to remove the node once the correct information is found

		vector<Node*> vNodes;
		vNodes = removeInorderNHelper(nTh, root, vNodes);
		if (vNodes.size() <= nTh)
		{
			cout << "unsuccessful" << endl;
		}
		else
		{
			removeID(vNodes.at(nTh)->gatorID, vNodes.at(nTh), nullptr);
		}
	}
	vector<Node*> removeInorderNHelper(int nTh, Node* node, vector<Node*> vNodes)
	{
		// Helper function to removeInorderN so I can store all of the nodes into one vector and use that to easily grab a pointer directly
		// to the node I want to delete
		if (node == nullptr)
		{
			return vNodes;
		}
		else
		{
			if (node->left != nullptr)
			{
				vNodes = removeInorderNHelper(nTh, node->left, vNodes);
			}
			vNodes.push_back(node);
			if (node->right != nullptr)
			{
				vNodes = removeInorderNHelper(nTh, node->right, vNodes);
			}
		}
		return vNodes;
	}
};

bool verifyName(string name)
{
	// Checks for valid name
	// 1 reqs - must consist alphabetical characters or spaces
	for (unsigned int i = 0; i < name.length(); i++)
	{
		if (isalpha(name[i]) || name[i] == ' ')
		{
			// okay
		}
		else
		{
			return false;
		}
	}
	return true;
}
bool verifyID(string id)
{
	// Checks for valid ID
	// 2 reqs - 8 digits and numerical
	for (unsigned int i = 0; i < id.length(); i++)
	{
		if (id.length() != 8)
		{
			// invalid - must be 8 digit
			return false;
		}
		if (isdigit(id[i]) == false)
		{
			// invalid - must be numerical
			return false;
		}
	}
	return true;
}

int main()
{
	// Initiailize tree
	avlTree Tree;

	// Takes in number of commands
	string line;
	getline(cin, line);
	int numOfCommands = stoi(line);
	for (unsigned int i = 0; i < numOfCommands; i++)
	{
		string inputLine;
		getline(cin, inputLine);
		string command;
		istringstream stream(inputLine);
		getline(stream, command, ' ');
		// Iterate through the 9 possible functions and execute the proper function
		if (command == "insert")
		{
			// take in name and id - make and insert node
			string name;
			string idString;
			getline(stream, name, '"');
			getline(stream, name, '"');
			getline(stream, idString, ' ');
			getline(stream, idString, '\0');
			if (verifyName(name) && verifyID(idString))
			{
				int idInt = stoi(idString);
				Tree.inser(Tree.getHead(), name, idInt, idString);
			}
			else
			{
				cout << "unsuccessful" << endl;
			}
		}
		else if (command == "remove")
		{
			// take in 8 digit id number
			string idString;
			getline(stream, idString);
			if (verifyID(idString))
			{
				int idInt = stoi(idString);
				Tree.removeID(idInt, Tree.getHead(), nullptr);
			}
			else
			{
				cout << "unsuccessful" << endl;
			}
		}
		else if (command == "search")
		{
			// if given id search name, if name search id
			string nextChars;
			getline(stream, nextChars, ' ');
			if (nextChars[0] == '"')
			{
				// Search by name not id
				string name = nextChars.substr(1, nextChars.size() - 2);
				if (verifyName(name))
				{
					Tree.searchNAME(name);
				}
				else
				{
					cout << "unsuccessful" << endl;
				}
			}
			else
			{
				// Search by id not name
				if (verifyID(nextChars))
				{
					int idInt = stoi(nextChars);
					Tree.searchID(idInt);
				}
				else
				{
					cout << "unsuccessful" << endl;
				}
			}
		}
		else if (command == "printInorder")
		{
			Tree.printInorder(Tree.getHead());
			cout << endl;
		}
		else if (command == "printPreorder")
		{
			Tree.printPreorder(Tree.getHead());
			cout << endl;
		}
		else if (command == "printPostorder")
		{
			Tree.printPostorder(Tree.getHead());
			cout << endl;
		}
		else if (command == "printLevelCount")
		{
			Tree.printLevelCount();
		}
		else if (command == "removeInorder")
		{
			// take in number for nth node
			string nthNode;
			getline(stream, nthNode);
			bool validNum = true;
			for (unsigned int j = 0; j < nthNode.length(); j++)
			{
				if (!isdigit(nthNode[j]))
				{
					validNum = false;
				}
			}
			if (validNum)
			{
				int nTh = stoi(nthNode);
				Tree.removeInorderN(nTh);
			}
			else
			{
				cout << "unsuccessful" << endl;
			}
		}
	}
}