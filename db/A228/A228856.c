/*
 * A228856  Nodes of tree generated as follows: (1,2) is an edge, and if (x,y)
 *          is an edge, then (y,y+x), (y,2y+x), and (y,3y+x) are edges.
 * https://oeis.org/A228856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A228856 – nodes of a tree generated as follows:
   (1,2) is an edge, and if (x,y) is an edge, then (y,y+x),
   (y,2y+x), and (y,3y+x) are edges.
   The function returns the n‑th node in sorted ascending order
   (1‑based index).  The algorithm performs a breadth‑first search
   using a min‑heap of edges; duplicate nodes are suppressed.  This
   implementation works for all `seq_t` sizes supported by the
   build. */

#include <stdlib.h>

/* Edge stored in the priority queue. */
struct Edge { seq_t x; seq_t y; };

/* ---------- Min‑heap of edges: ordered by y ---------- */
static struct Edge *edge_heap = NULL;          /* heap array */
static size_t heap_size = 0, heap_cap = 0;      /* current size & capacity */

/* Push a new edge onto the heap. */
static void heap_push(seq_t x, seq_t y)
{
    if (heap_size == heap_cap) {
        size_t new_cap = heap_cap ? heap_cap << 1 : 4;
#if defined(__cplusplus)
        struct Edge *tmp = new struct Edge[new_cap];
        for (size_t i = 0; i < heap_size; ++i) tmp[i] = edge_heap[i];
        delete [] edge_heap;
#else
        struct Edge *tmp = (struct Edge *)realloc(edge_heap, new_cap * sizeof(*edge_heap));
#endif
        edge_heap = tmp;
        heap_cap = new_cap;
    }
    edge_heap[heap_size].x = x;
    edge_heap[heap_size].y = y;

    /* sift‑up */
    size_t i = heap_size++;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (edge_heap[p].y <= edge_heap[i].y) break;
        struct Edge t = edge_heap[i];
        edge_heap[i] = edge_heap[p];
        edge_heap[p] = t;
        i = p;
    }
}

/* Pop the edge with smallest y. */
static struct Edge heap_pop(void)
{
    struct Edge e = edge_heap[0];
    edge_heap[0] = edge_heap[heap_size - 1];
    heap_size--;

    /* sift‑down */
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        if (l >= heap_size) break;
        size_t r = l + 1;
        size_t s = (r < heap_size && edge_heap[r].y < edge_heap[l].y) ? r : l;
        if (edge_heap[i].y <= edge_heap[s].y) break;
        struct Edge t = edge_heap[i];
        edge_heap[i] = edge_heap[s];
        edge_heap[s] = t;
        i = s;
    }
    return e;
}

/* ---------- Dynamic array of discovered nodes ---------- */
static seq_t *nodes = NULL;          /* sorted ascending array of nodes */
static size_t nodes_size = 0, nodes_cap = 0;

/* Append a node (assumed to be larger than all previous nodes). */
static void add_node(seq_t v)
{
    if (nodes_size == nodes_cap) {
        size_t new_cap = nodes_cap ? nodes_cap << 1 : 4;
#if defined(__cplusplus)
        seq_t *tmp = new seq_t[new_cap];
        for (size_t i = 0; i < nodes_size; ++i) tmp[i] = nodes[i];
        delete [] nodes;
#else
        seq_t *tmp = (seq_t *)realloc(nodes, new_cap * sizeof(*nodes));
#endif
        nodes = tmp;
        nodes_cap = new_cap;
    }
    nodes[nodes_size++] = v;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Lazy initialization of the data structures. */
    static int initialized = 0;
    if (!initialized) {
        /* Initial node 1 */
        nodes_cap = 4;
#if defined(__cplusplus)
        nodes = new seq_t[nodes_cap];
#else
        nodes = malloc(nodes_cap * sizeof(*nodes));
#endif
        nodes_size = 0;
        add_node((seq_t)1);

        /* Initial edge (1,2) */
        heap_cap = 4;
#if defined(__cplusplus)
        edge_heap = new struct Edge[heap_cap];
#else
        edge_heap = (struct Edge *)malloc(heap_cap * sizeof(*edge_heap));
#endif
        heap_size = 0;
        heap_push((seq_t)1, (seq_t)2);

        initialized = 1;
    }

    /* Generate nodes until we have at least n of them. */
    while (nodes_size < (size_t)n) {
        if (heap_size == 0) break; /* safety */
        struct Edge e = heap_pop();
        seq_t x = e.x;
        seq_t y = e.y;

        /* Add node y if it is new. */
        if (y != nodes[nodes_size - 1]) {
            add_node(y);
        }

        /* Generate child edges from (y, ?). */
        seq_t y1 = y + x;       /* y + x        */
        seq_t y2 = y + y + x;   /* 2y + x       */
        seq_t y3 = y + y + y + x; /* 3y + x    */

        heap_push(y, y1);
        heap_push(y, y2);
        heap_push(y, y3);
    }

    return nodes[(size_t)(n - 1)];
}
