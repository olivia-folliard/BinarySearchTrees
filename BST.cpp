//Olivia Folliard and Kasey Quinones
#include "BST.hpp"
#include <iostream>
#include <cstdlib>
using namespace std;

BST::BST() {
	root = NULL;
}

BST::BST(string title, string author, int yr, string pub, long long isbn, float rate) {
	root = new BSTNode(title, author, yr, pub, isbn, rate);
}

BST::~BST() {
	clearTree();
}

void BST::insert(string title, string author, int yr, string pub, long long isbn, float rate) {
	BSTNode *newNode = new BSTNode(title,author,yr,pub,isbn,rate);
    if (root == NULL){
        root = newNode;
        return;
    }else{
        BSTNode *curr = root;
        while(curr != NULL){
            if (*(newNode -> book) < *(curr -> book)){
                if (curr -> left == NULL){
                    newNode -> parent = curr;
                    curr -> left = newNode;
                    setHeight(newNode);
                    return;
                }
                curr = curr -> left;
            }else {
                if (curr->right == NULL) {
                    newNode->parent = curr;
                    curr->right = newNode;
                    setHeight(newNode);
                    return;
                }
                curr = curr->right;
            }
        }
    }
}

BSTNode *BST::find(string title, string author) {
    BSTNode *curr = root;
    Book want(title, author, 0, " ", 0, 0.0);
    while(curr) {
        if (*(curr->book) == want) {
            return curr;
        } else if (want > *(curr->book)) {
            curr = curr->right;
        } else {
            curr = curr->left;
        }
    }
	return NULL;
}

BSTNode *BST::find(string title, string author, BSTNode *start) {
    Book want(title, author, 0, " ", 0, 0.0);
    while(start){
        if (*(start -> book) == want){
            return start;
        }
        else if (want > *(start -> book)){
            start = start -> right;
        }
        else{
            start = start -> left;
        }
    }
	return NULL;
}

void BST::printTreeIO() {
	if (root == NULL) {
		cout << "Empty Tree" << endl;
	}else {
		cout << endl << "Printing In-Order:" << endl;
		printTreeIO(root);
	}
}

void BST::printTreeIO(BSTNode *node) {
	if (node == NULL){
        return;
    }
    printTreeIO(node -> left);
    node -> printNode(false);
    printTreeIO(node -> right);
}

void BST::printTreePre() {
	if (root == NULL) {
		cout << "Empty Tree" << endl;
	}else {
		cout << endl << "Printing Pre-Order:" << endl;
		printTreePre(root);
	}
}

void BST::printTreePre(BSTNode *node) {
	if (node == NULL){
        return;
    }
    node -> printNode(false);
    printTreePre(node -> left);
    printTreePre(node -> right);
}

void BST::printTreePost() {
	if (root == NULL) {
		cout << "Empty Tree" << endl;
	}
	else {
		cout << endl<<"Printing Post-Order:" <<endl;
		printTreePost(root);
	}
}

void BST::printTreePost(BSTNode *node) {
    if (node == NULL){
        return;
    }
    printTreePost(node -> left);
    printTreePost(node -> right);
    node ->printNode(false);
}

void BST::clearTree() {
	if (root == NULL) {
		cout << "Tree already empty" << endl;
	}
	else {
		cout << endl << "Clearing Tree:" << endl;
		clearTree(root);
		root = NULL;
	}
}

void BST::clearTree(BSTNode *node) {
	if (node == NULL) {
		return;
	}
	else {
		clearTree(node->left);
		node->left = NULL;
		clearTree(node->right);
		node->right = NULL;
		node->printNode(true);
		delete node;
	}
}

bool BST::checkOut(string title, string author) {
    BSTNode *toCheckOut = find(title, author);
    if (toCheckOut != NULL){
        if(toCheckOut -> book -> checked_out == false) {
            toCheckOut->book->checked_out = true;
            return true;
        }
        else{
            while(toCheckOut){
                if (toCheckOut -> right == NULL){
                    return false;
                }
                toCheckOut = toCheckOut -> right;
                toCheckOut = find(title, author, toCheckOut);
                if (toCheckOut){
                    if(toCheckOut -> book -> checked_out == false){
                        toCheckOut->book->checked_out = true;
                        return true;
                    }
                }
                else{
                    return false;
                }
            }
        }
    }
	return false;
}

bool BST::checkIn(string title, string author) {
    BSTNode *toCheckIn = find(title, author);
    if (toCheckIn != NULL){
        if(toCheckIn -> book -> checked_out == true) {
            toCheckIn->book->checked_out = false;
            return true;
        }
        else{
            while(toCheckIn){
                if (toCheckIn -> right == NULL){
                    return false;
                }
                toCheckIn = toCheckIn -> right;
                toCheckIn = find(title, author, toCheckIn);
                if (toCheckIn){
                    if(toCheckIn -> book -> checked_out == true){
                        toCheckIn->book->checked_out = false;
                        return true;
                    }
                }
                else{
                    return false;
                }
            }
        }
    }
	return false;
}

void BST::updateRating(string title, string author, float newRating) {
    BSTNode *match = find(title, author);
    if (!match){
        return;
    }
    while(match != NULL){
        match -> book -> rating = newRating;
        //goes right because we handle equality to the right,
        // so if we're looking for an equal book we will find it on the right
        match = match -> right;
        match = find(title,author,match);
    }
    return;
}


BSTNode *BST::removeNoKids(BSTNode *node) {
    if (node == root) {
        root = NULL;
        return node;
    }
    if (*(node-> parent->book)>*(node->book)){
        node->parent->left = NULL;
    }else{
        node->parent->right = NULL;
    }
    return node;
}


BSTNode *BST::removeOneKid(BSTNode *node, bool leftFlag) {
    if (node == NULL) {
        return NULL;
    }
    if ((node->left == NULL && node->right != NULL) || (node->left != NULL && node->right == NULL)) {
        BSTNode *child = (node->left != NULL) ? node->left : node->right;


        if (node->parent != NULL) {
            if (leftFlag) {
                node->parent->left = child;
            } else {
                node->parent->right = child;
            }
            child->parent = node->parent;
        } else {
            root = child;
            child->parent = NULL;
        }
    }
    return node;
}

BSTNode *BST::remove(string title, string author) {
    BSTNode *nodeToRemove = find(title, author);
    if (nodeToRemove == NULL) {
        return NULL;
    }
    if (nodeToRemove->left == NULL && nodeToRemove->right == NULL) {
        removeNoKids(nodeToRemove);
    } else if ((nodeToRemove->left == NULL && nodeToRemove->right != NULL) || (nodeToRemove->left != NULL && nodeToRemove->right == NULL)) {
        bool leftFlag = (nodeToRemove->left != NULL);
        removeOneKid(nodeToRemove, leftFlag);
    } else {
        BSTNode *nextNode = nodeToRemove->right;
        while (nextNode->left != NULL) {
            nextNode = nextNode->left;
        }
        nodeToRemove->book = nextNode->book;
        remove(nextNode->book->title, nextNode->book->author);
    }
    setHeight(nodeToRemove);
    return nodeToRemove;
}

void BST::setHeight(BSTNode *node) {
    node -> height = 1;
    BSTNode *curr = node;
    while (curr -> parent != NULL){
        curr = curr -> parent;
        if (curr -> left == NULL){
            curr -> height = (curr -> right -> height) + 1;
        }
        else if (curr -> right == NULL){
            curr -> height = (curr -> left -> height) + 1;
        }
        else {
            if (curr->left->height > curr->right->height) {
                int newH = (curr->left->height) + 1;
                if (newH == curr->height) {
                    return;
                }
                else {
                    curr->height = newH;
                }
            }
            else {
                int newH = (curr->right->height) + 1;
                if (newH == curr->height) {
                    return;
                }
                else {
                    curr->height = newH;
                }
            }
        }
    }
}