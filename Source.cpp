#include<iostream>
#include<vector>
#include<map>
#include <stack>
#include <string>

using namespace std;

struct Node {
public:
	string name;
	map<Node*, double> l;

	Node() :name("NILL") {};
	Node(string s) : name(s) {};

	Node(const Node& x) {
		name = x.name;
		for (auto i : x.l) {
			l[i.first] = i.second;
		}
	}

	Node& operator = (const Node& x) {
		name = x.name;
		for (auto i : x.l) {
			l[i.first] = i.second;
		}
		return *this;
	}
	void add_edge(Node* d, double w) {
		l[d] = w;
	}

	void remove_edge(Node* t) {
		l.erase(t);
	}

	void del_node() {
		l.clear();
	}

	bool adj_weight(Node* t, double w) {
		if (l[t] != NULL) {
			l[t] = w;
			return true;
		}
		else {
			return false;
		}
	}

	void print() {
		cout << name << " ";
	}

	bool isEdge(Node* d) {
		if (l[d] != NULL) {
			return true;
		}
		else {
			return false;
		}
	}
	bool operator == (const Node d) const {
		return name == d.name;
	}
	bool operator != (const Node d) const {
		return name != d.name;
	}
	bool operator <(const Node d) const {
		return name[0] < d.name[0];
	}
	bool operator <=(const Node d) const {
		return name[0] <= d.name[0];
	}
	bool operator >(const Node d) const {
		return name[0] > d.name[0];
	}
	bool operator >=(const Node d) const {
		return name[0] >= d.name[0];
	}


};

enum typegraph { directed, undirected };
class Graph {
public:
	map<Node*, int> nodes;
	unsigned size;
	Node* NILL;
	typegraph type;

	Graph(typegraph z) :size(0), type(z), NILL(new Node()) {};

	void addnode(Node* n) {
		size++;
		nodes[n] = 1;
	}

	bool addedge(Node* s, Node* d, double w) {
		if (nodes[s] == NULL || nodes[d] == NULL) {
			return false;
		}
		if (type == directed) {
			(*s).add_edge(d, w);
			return true;
		}
		else {
			(*s).add_edge(d, w);
			(*d).add_edge(s, w);
			return true;
		}
	}

	bool delEdge(Node* s, Node* d) {

		if (type == directed) {
			(*s).remove_edge(d);
			return true;
		}
		else {
			(*s).remove_edge(d);
			(*d).remove_edge(s);
		}
	}

	void delNode(Node* s) {
		if (type == directed) {
			for (auto i : nodes) {
				Node* m = i.first;
				(*m).remove_edge(s);
			}
			(*s).del_node();
		}
		else {
			for (auto i : s->l) {
				Node* m = i.first;
				(*m).remove_edge(s);
			}
			(*s).del_node();
		}
	}
	bool adjustweight(Node* s, Node* d, double w) {
		if (type == directed) {
			if ((*s).isEdge(d)) {
				(*s).adj_weight(d, w);
				return true;
			}
			else {
				return false;
			}
		}
		else {
			if ((*s).isEdge(d)) {
				(*s).adj_weight(d, w);
				(*d).adj_weight(s, w);
				return true;
			}
			else {
				return false;
			}
		}
	}

	void DFSs(Node* w, map<Node*, bool>& v) {
		stack<Node*> s;
		s.push(w);
		v[w] = true;
		Node* c;
		while (!s.empty()) {
			c = s.top();
			s.pop();
			(*c).print();
			for (auto i : c->l) {
				if (v[i.first] != true) {
					v[i.first] = true;
					s.push(i.first);
				}
			}
		}
	}

	void DFS(Node* s) {
		map <Node*, bool> visited;
		for (auto i : this->nodes) {
			visited[i.first] = false;
		}
		DFSs(s, visited);

	}
	bool Cycle() {
		if (type == directed) {
			map<Node*, bool>visited;
			map<Node*, bool>inprogress;
			for (auto i : this->nodes) {
				visited[i.first] = false;
				inprogress[i.first] = false;
			}


			map<Node*, Node*>parent;
			for (auto i : this->nodes) {
				if (visited[i.first] == false) {

					if (DFScycleU(i.first, visited, parent, inprogress)) {
						return true;
					}
				}
			}
			return false;
		}
		else {
			map<Node*, bool>visited;
			map<Node*, bool>inprogress;
			for (auto i : this->nodes) {
				visited[i.first] = false;
				inprogress[i.first] = false;
			}
			map<Node*, Node*>parent;

		}
	}
	bool DFScycleU(Node* x, map<Node*, bool>visited, map<Node*, Node*>parent, map<Node*, bool>inprogress) {
		if (type == directed) {
			visited[x] = true;
			stack<Node*> s;
			s.push(x);
			inprogress[x] = true;
			Node* c;
			while (!s.empty()) {
				c = s.top();
				s.pop();
				visited[c] = true;
				for (auto i : c->l) {
					if (inprogress[i.first] == true && visited[i.first] == false) {
						return true;
					}
					else {
						inprogress[i.first] = true;
						s.push(i.first);
					}
				}
			}
			return false;
		}
		else {
			visited[x] = true;
			stack<Node*> s;
			s.push(x);
			inprogress[x] = true;
			Node* c;
			while (!s.empty()) {
				c = s.top();
				s.pop();
				visited[c] = true;
				for (auto i : c->l) {
					if (inprogress[i.first] == true && visited[i.first] == false && parent[i.first] != c) {
						return true;
					}
					else {
						inprogress[i.first] = true;
						parent[i.first] = c;
						s.push(i.first);
					}
				}
			}
			return false;
		}
	}

	void ShortestPath(Node* s, Node* d) {

		map<Node*, double> distance;
		map<Node*, Node*> parent;
		for (auto i : nodes) {
			distance[i.first] = NULL;
			parent[i.first] = NILL;
		}
		distance[s] = 0;
		for (int l = 0; l < size; l++) {
			for (auto i : nodes) {
				for (auto j : i.first->l) {
					if (j.first != s && (distance[j.first] == NULL || (distance[j.first] >= distance[i.first] + j.second))) {
						distance[j.first] = distance[i.first] + j.second;
						parent[j.first] = i.first;
					}
				}
			}
		}
		for (auto i : nodes) {
			for (auto j : i.first->l) {
				if (distance[j.first] > distance[i.first] + j.second) {
					cout << "NEGATIVE-WEIGHT-CYCLE" << endl;
					return;
				}
			}
		}
		Node* t = d;
		stack<Node*> m;
		while (t != NILL) {
			m.push(t);
			t = parent[t];
		}
		while (!m.empty()) {
			t = m.top();
			(*t).print(); cout << " ";
			m.pop();
		}
		cout << endl;

	}
	void topologicalSort() {
		if (type == directed) {
			if (Cycle() == true) {
				cout << "GRAPH ISN'T ACYCLIC" << endl;
				return;
			}
			else {
				map<Node*, bool> visited;
				for (auto i : nodes) {
					visited[i.first] = false;
				}
				map<Node*, bool> inprogress;
				for (auto i : nodes) {
					inprogress[i.first] = false;
				}
				stack<Node*> s;
				for (auto i : nodes) {
					if (inprogress[i.first] == false) {
						top(i.first, visited);
					}
				}
			}
		}
	}
	void top(Node* w, map<Node*, bool> v) {
		stack<Node*> s;
		stack<Node*>q;
		s.push(w);
		v[w] = true;
		Node* c;
		while (!s.empty()) {
			c = s.top();
			s.pop();
			q.push(c);
			//(*c).print();
			for (auto i : c->l) {
				if (v[i.first] != true) {
					v[i.first] = true;
					s.push(i.first);
				}
			}
			while (!q.empty()) {
				Node* x = q.top();
				(*x).print(); cout << " ";
				q.pop();
			}
		}
	}

};
int main() {
	Node a("a");
	Node b("b");
	Node c("c");
	Node d("d");
	Graph G(directed);
	G.addnode(&a);
	G.addnode(&b);
	G.addnode(&c);
	G.addnode(&d);
	G.addedge(&a, &b, 1.2);
	G.addedge(&b, &c, 2.2);
	G.addedge(&b, &d, 0.2);
	G.addedge(&d, &a, 3);
	G.delEdge(&d, &a);
	G.adjustweight(&b, &d, 0.1);
	G.addedge(&c, &d, 1);
	cout << "DFS: "; G.DFS(&a);
	cout << endl;
	cout << "Cycle exists: " << G.Cycle() << endl;
	cout << "Shortest path a to d: "; G.ShortestPath(&a, &d); cout << endl;;
	G.topologicalSort();
	return 0;
}
