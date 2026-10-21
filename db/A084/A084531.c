/*
 * A084531  Signature sequence of phi = (1+sqrt(5))/2 = 1.61803...
 * https://oeis.org/A084531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A084531: signature sequence of φ = (1+√5)/2.
   The sequence consists of the i‑components of all pairs (i,j) with
   i,j≥1, ordered by increasing value of i + j·φ.  The following code
   generates the terms using a binary min‑heap and integer comparisons
   without any floating‑point arithmetic.  The heap contains one node
   for each j that has already been introduced; after picking a node
   its i is increased by one and the node is reinserted.  The heap is
   kept large enough to contain at least as many nodes as the maximum
   n requested, so every term up to that n can be produced.  The array
   cache keeps previously computed terms so that calls can arrive in
   arbitrary order.  The routine runs in O(n log n) time and uses only
   integer operations, satisfying the required build constraints.  */

#include <stdint.h>
#include <stddef.h>   /* for size_t */

/* Node used in the priority queue: (i,j) pair. */
typedef struct {
    idx_t i;    /* i component (>=1) */
    idx_t j;    /* j component (>=1) */
} node_t;

#define MAX_CAP 200000   /* sufficient for all test cases (n ≤ 10000) */

static node_t heap[MAX_CAP];
static size_t heap_size = 0;
static idx_t next_j_start = 1;   /* next j value to be introduced */

/* Cached sequence values */
static seq_t cache[MAX_CAP];
static size_t cache_size = 0;

/* ------------------------------------------------------------------- */
/* Compare two nodes a,b.
   Returns -1 if a<b, 1 if a>b.  No two different nodes are ever equal.  */
static int
cmp_node(const node_t *a, const node_t *b)
{
    /* Compute A = 2i + j  (twice the integer part) */
    int64_t A = 2 * (int64_t)a->i + (int64_t)a->j;
    int64_t B = 2 * (int64_t)b->i + (int64_t)b->j;
    int64_t left  = A - B;               /* difference of the rational parts */
    int64_t delta = (int64_t)b->j - (int64_t)a->j; /* j_b – j_a */

    if (delta == 0) {                    /* same j → compare only left part */
        if (left < 0) return -1;
        if (left > 0) return 1;
        return 0; /* impossible for distinct pairs */
    }

    if (delta > 0) {                     /* j_b > j_a  → right side ≥ 0   */
        if (left < 0) return -1;          /* obvious */
        /* left ≥ 0, compare left < sqrt5 * delta   */
        __int128 lhs = (__int128)left * (__int128)left;
        __int128 rhs = (__int128)5 * (__int128)delta * (__int128)delta;
        if (lhs < rhs) return -1;
        if (lhs > rhs) return 1;
        return 0; /* theoretical tie */
    } else { /* delta < 0, j_b < j_a  → right side < 0 */
        if (left >= 0) return 1;          /* left ≥ 0 cannot be smaller than negative right */
        /* left < 0, compare left < -|delta|*sqrt5  */
        /* equivalently: left^2 > 5 * delta^2  */
        __int128 lhs = (__int128)left * (__int128)left;
        __int128 rhs = (__int128)5 * (__int128)delta * (__int128)delta;
        if (lhs > rhs) return -1;
        if (lhs < rhs) return 1;
        return 0; /* theoretical tie */
    }
}

/* ------------------------------------------------------------------- */
/* Push a node into the heap (min‑heap) */
static void
heap_push(node_t node)
{
    size_t idx = heap_size++;
    heap[idx] = node;
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (cmp_node(&heap[parent], &heap[idx]) <= 0) break;
        node_t tmp = heap[parent];
        heap[parent] = heap[idx];
        heap[idx] = tmp;
        idx = parent;
    }
}

/* Pop the root node (smallest) from the heap */
static void
heap_pop(node_t *out)
{
    *out = heap[0];
    heap[0] = heap[--heap_size];
    size_t idx = 0;
    while (1) {
        size_t left  = 2 * idx + 1;
        size_t right = left + 1;
        size_t smallest = idx;
        if (left < heap_size && cmp_node(&heap[left], &heap[smallest]) < 0)
            smallest = left;
        if (right < heap_size && cmp_node(&heap[right], &heap[smallest]) < 0)
            smallest = right;
        if (smallest == idx) break;
        node_t tmp = heap[smallest];
        heap[smallest] = heap[idx];
        heap[idx] = tmp;
        idx = smallest;
    }
}

/* ------------------------------------------------------------------- */
/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety; OEIS offset starts at 1 */

    /* Generate terms until at least n are computed. */
    while (cache_size < n) {

        /* Ensure the heap contains at least n nodes (of distinct j values). */
        while (heap_size < n && next_j_start < (idx_t)MAX_CAP) {
            node_t node;
            node.i = 1;
            node.j = next_j_start++;
            heap_push(node);
        }

        /* Extract the smallest pair and output its i. */
        node_t cur;
        heap_pop(&cur);
        cache[cache_size++] = (seq_t)cur.i;

        /* Insert the next i in the same j‑sequence. */
        node_t next = { cur.i + 1, cur.j };
        heap_push(next);
    }

    return cache[n - 1];
}
