//============================================================================
// Name        : CFS.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

#include <unistd.h>


#define  TSQ  20
class Node{
public:
	enum COLOR{RED,BLACK};
	int pid;
	COLOR color;
	class Node *parent,*left,*right;

	int vruntime,totalruntime;
	Node(){
		pid =-1;
		color = RED;
		parent=left=right=NULL;
		vruntime=0;

	}
	Node(int ippid){
		pid =ippid;
		color = RED;
		parent=left=right=NULL;
		vruntime=0;

	}

	void print()
	{
		cout<<"pid:color:vruntime:totalruntime::::"<<pid<<":"<<color<<":"<<vruntime<<":"<<totalruntime<<endl;
		return;
	}
};



typedef Node * NodePtr;

class RBTree{
public:

	Node * root, *TNULL;
	RBTree()
	{

		TNULL = new Node;
		TNULL->color = Node::BLACK/* 0*/;
		TNULL->left = nullptr;
		TNULL->right = nullptr;
		TNULL->pid=-1;
		root = TNULL;
	}

	void addnewnode(int pid, int totalruntime)
	{
		insert(0,totalruntime,pid);//vruntime =0 initially
		cout<<"Process Id::"<<pid<<endl;
	}
	void addnode(Node oNode){
		insert(oNode.vruntime,oNode.totalruntime,oNode.pid);//vruntime =0 initially
	}


	void deleteFix(NodePtr x)
	{
		NodePtr s;
		while (x != root && x->color == Node::BLACK/* 0*/)
		{
			if (x == x->parent->left)
			{
				s = x->parent->right;
				if (s->color == Node::RED/*1*/)
				{
					s->color = Node::BLACK/* 0*/;
					x->parent->color = Node::RED/*1*/;
					leftRotate(x->parent);
					s = x->parent->right;
				}

				if (s->left->color == Node::BLACK/* 0*/ && s->right->color == Node::BLACK/* 0*/)
				{
					s->color = Node::RED/*1*/;
					x = x->parent;
				}
				else
				{
					if (s->right->color == Node::BLACK/* 0*/)
					{
						s->left->color = Node::BLACK/* 0*/;
						s->color = Node::RED/*1*/;
						rightRotate(s);
						s = x->parent->right;
					}

					s->color = x->parent->color;
					x->parent->color = Node::BLACK/* 0*/;
					s->right->color = Node::BLACK/* 0*/;
					leftRotate(x->parent);
					x = root;
				}
			}
			else
			{
				s = x->parent->left;
				if (s->color == 1)
				{
					s->color = Node::BLACK/* 0*/;
					x->parent->color = Node::RED/*1*/;
					rightRotate(x->parent);
					s = x->parent->left;
				}

				if (s->right->color == Node::BLACK/* 0*/ && s->right->color == Node::BLACK/* 0*/)
				{
					s->color = Node::RED/*1*/;
					x = x->parent;
				}
				else
				{
					if (s->left->color == Node::BLACK/* 0*/)
					{
						s->right->color = Node::BLACK/* 0*/;
						s->color = Node::RED/*1*/;
						leftRotate(s);
						s = x->parent->left;
					}

					s->color = x->parent->color;
					x->parent->color = Node::BLACK/* 0*/;
					s->left->color = Node::BLACK/* 0*/;
					rightRotate(x->parent);
					x = root;
				}
			}
		}
		x->color = Node::BLACK/* 0*/;
	}

	void rbTransplant(NodePtr u, NodePtr v)
	{
		if (u->parent == nullptr)
		{
			root = v;
		}
		else if (u == u->parent->left)
		{
			u->parent->left = v;
		}
		else
		{
			u->parent->right = v;
		}
		v->parent = u->parent;
	}

	void deleteNodeHelper(NodePtr node, int oldvruntime,int pid)
	{
		NodePtr z = TNULL;
		NodePtr x, y;
		/*while (node != TNULL)
		{
			if (node->vruntime == oldvruntime)
			{
				z = node;
			}

			if (node->vruntime <= oldvruntime)
			{
				node = node->right;
			}
			else
			{
				node = node->left;
			}
		}*/

		z=LMostNode();

		if (z == TNULL)
		{
			cout << "vruntime not found in the tree" << endl;
			return;
		}

		y = z;
		int y_original_color = y->color;
		if (z->left == TNULL)
		{
			x = z->right;
			rbTransplant(z, z->right);
		}
		else if (z->right == TNULL)
		{
			x = z->left;
			rbTransplant(z, z->left);
		}
		else
		{
			y = minimum(z->right);
			y_original_color = y->color;
			x = y->right;
			if (y->parent == z)
			{
				x->parent = y;
			}
			else
			{
				rbTransplant(y, y->right);
				y->right = z->right;
				y->right->parent = y;
			}

			rbTransplant(z, y);
			y->left = z->left;
			y->left->parent = y;
			y->color = z->color;
		}
		delete z;
		if (y_original_color == Node::BLACK/* 0*/)
		{
			deleteFix(x);
		}
	}


	void deleteNode(int oldvruntime,int pid)
	{
		deleteNodeHelper(this->root,  oldvruntime, pid);
	}


	//------------
	void leftRotate(NodePtr x)
	{
		NodePtr y = x->right;
		x->right = y->left;
		if (y->left != TNULL)
		{
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr)
		{
			this->root = y;
		}
		else if (x == x->parent->left)
		{
			x->parent->left = y;
		}
		else
		{
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

	void rightRotate(NodePtr x)
	{
		NodePtr y = x->left;
		x->left = y->right;
		if (y->right != TNULL)
		{
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr)
		{
			this->root = y;
		}
		else if (x == x->parent->right)
		{
			x->parent->right = y;
		}
		else
		{
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}

	void insertFix(NodePtr k)
	{
		//cout<<"balance tree"<<endl;
		NodePtr u;
		while (k->parent->color == Node::RED/*1*/)
		{
			//cout<<"balance tree inside while"<<endl;
			if (k->parent == k->parent->parent->right)
			{
				//cout<<"balance tree1"<<endl;
				u = k->parent->parent->left;
				//cout<<"balance tree4::"<<u->color<<endl;
				if (u->color == Node::RED/*1*/)
				{
					//cout<<"balance tree2"<<endl;
					u->color =Node::BLACK/* 0*/;
					k->parent->color = Node::BLACK/* 0*/;
					k->parent->parent->color = Node::RED/*1*/;
					k = k->parent->parent;
				}
				else
				{
					//cout<<"balance tree3"<<endl;
					if (k == k->parent->left)
					{
						k = k->parent;
						rightRotate(k);
					}
					k->parent->color = Node::BLACK/* 0*/;
					k->parent->parent->color = Node::RED/*1*/;
					leftRotate(k->parent->parent);
				}
			}
			else
			{

				u = k->parent->parent->right;

				if (u->color == Node::RED/*1*/)
				{
					u->color =Node::BLACK/* 0*/;
					k->parent->color = Node::BLACK/* 0*/;
					k->parent->parent->color = Node::RED/*1*/;
					k = k->parent->parent;
				}
				else
				{
					if (k == k->parent->right)
					{
						k = k->parent;
						leftRotate(k);
					}
					k->parent->color = Node::BLACK/* 0*/;
					k->parent->parent->color = Node::RED/*1*/;
					rightRotate(k->parent->parent);
				}
			}
			if (k == root)
			{
				break;
			}
		}
		//cout<<"balance tree end"<<endl;
		root->color = Node::BLACK/* 0*/;
	}
	void insert(int vruntime , int totalruntime, int pid)
	{
		cout<<"insert process "<<endl;
		cout<<vruntime<<endl;
		cout<<totalruntime<<endl;

		NodePtr node = new Node;
		node->parent = nullptr;
		node->vruntime = vruntime;
		node->totalruntime = totalruntime;
		node->left = TNULL;
		node->right = TNULL;
		node->color =Node::RED;// 1;
		node->pid =pid;// 1;


		NodePtr y = nullptr;
		NodePtr x = this->root;

		while (x != TNULL)
		{
			y = x;
			if (node->vruntime < x->vruntime)
			{
				x = x->left;
			}
			else
			{
				x = x->right;
			}
		}

		node->parent = y;
		if (y == nullptr)
		{
			root = node;
		}
		else if (node->vruntime < y->vruntime)
		{
			y->left = node;
		}
		else
		{
			y->right = node;
		}

		if (node->parent == nullptr)
		{
			node->color = Node::BLACK;//0;
			return;
		}

		if (node->parent->parent == nullptr)
		{
			return;
		}
		/*cout<<"----------------"<<endl;
				node->print();
		cout<<"----------------"<<endl;
		printTree();
		cout<<"----------------"<<endl;*/

		insertFix(node);
	}


	NodePtr LMostNode()
	{
		cout<<"Find leftmode node.."<<endl;
		NodePtr node =  root;
		if(node == TNULL){
			cout<<"Empty tree"<<endl;
			return TNULL;
		}
		while (node->left != TNULL)
		{
			node = node->left;
		}
		return node;
	}
	NodePtr minimum(NodePtr node)
	{
		while (node->left != TNULL)
		{
			node = node->left;
		}
		return node;
	}




	void printHelper(NodePtr root, string indent, bool last)
	{
		if(root->pid==-1){
			return;
		}
		if (root != NULL)
		{
			cout << indent;
			if (last)
			{
				cout << "R----";
				indent += "   ";
			}
			else
			{
				cout << "L----";
				indent += "|  ";
			}

			string sColor = root->color == (Node::RED) ? "RED" : "BLACK";
			cout << root->pid<<", vr:"<<root->vruntime<<":"<<root->totalruntime<< "(" << sColor << ")" << endl;
			printHelper(root->left, indent, false);
			printHelper(root->right, indent, true);
		}
		return;
	}

	void printTree()
	{
		if (root)
		{
			printHelper(this->root, "", true);
		}
	}

};
class CFSSched{
public:
	RBTree oRBTree;
	Node scheduler()
	{
		cout<<">>>>>>>>>"<<endl;

		return *(oRBTree.LMostNode());
	}

};

class MainSystem{

public:
	int pidgencount;
	int totalprocess;
	CFSSched oSch;

	MainSystem()
	{
		pidgencount=0;
		totalprocess=0;

	}

	void incrtotalprocess(){
		totalprocess++;
	}
	void decrtotalprocess(int &i)
	{
		totalprocess--;
		if(totalprocess<0){
			totalprocess=-1;
		}
	}


	void addprocess(int totalruntime){
		totalprocess++;
		oSch.oRBTree.addnewnode(pidgencount++,totalruntime);
		cout<<"added Process node in RBT"<<endl;
	}

	void dispatcher(Node elNode){

		Node tmpcopyelNode = elNode;
		tmpcopyelNode.vruntime += TSQ/totalprocess;
		cout<<"Execute pid::"<<tmpcopyelNode.pid<<" timesliceofquantum::"<<(TSQ/totalprocess)<<endl;
		tmpcopyelNode.print();

		cout<<"delete from rbt"<<endl;
		oSch.oRBTree.deleteNode(elNode.vruntime,elNode.pid);
		if(tmpcopyelNode.vruntime<tmpcopyelNode.totalruntime){
			oSch.oRBTree.addnode(tmpcopyelNode);
		} else {
			cout<<"Process Completed "<<elNode.pid<<endl;
			totalprocess--;
		}

	}

	void start(){
		addprocess(100);
		addprocess(200);
		addprocess(50);
		addprocess(25);
		addprocess(96);
		oSch.oRBTree.printTree();
		//sleep(5);

		while(1){
			Node elNode=oSch.scheduler();
			cout<<"elNode.pid::"<<elNode.pid<<endl;
			if(elNode.pid == -1){
				break;
			}
			cout<<"eligible process::"<<endl;
			elNode.print();
			dispatcher(elNode);
			oSch.oRBTree.printTree();
			//sleep(1);
		}
		cout<<"CFS Algo End.."<<endl;

	}


};



int main() {
	cout << "CFS scheduling algo start.." << endl; // prints !!!Hello World!!!
	MainSystem oMainSystem;
	oMainSystem.start();
	return 0;
}
