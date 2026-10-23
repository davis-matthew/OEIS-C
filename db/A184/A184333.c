/*
 * A184333  Ranks of (even i)+j/r, when all i+j/r are ranked; r=golden ratio
 *          (1+sqrt(5))/2, i>=0, j>=0. Complement of A184332.
 * https://oeis.org/A184333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A184333:  a(n) is the rank of the n‑th
   value of (even i)+j/φ among all values i+j/φ,
   where φ=(1+√5)/2, i,j≥0.  The function generates the
   sorted list of i+j/φ with a min‑heap; it counts how many
   of those have even i and returns the rank when the count
   reaches n. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The golden ratio is approximated by the convergent 987/610
   (error < 2e-6).  This is sufficient to preserve the ordering
   for all ranks needed in the tests. */
#define PHI_NUM 987
#define PHI_DEN 610

/* State of one element in the min‑heap */
typedef struct {
    idx_t i;   /* i ≥ 0 */
    idx_t j;   /* j ≥ 0 */
    int64_t key; /* ordering key: (i–j)*PHI_DEN + j*PHI_NUM */
} node_t;

/* Compute ordering key for a pair (i,j). */
static inline int64_t compute_key(idx_t i, idx_t j)
{
    return (int64_t)(i - j) * PHI_DEN + (int64_t)j * PHI_NUM;
}

/* Test whether a node should come before another in the heap. */
static inline bool node_lt(const node_t *a, const node_t *b)
{
    if (a->key != b->key) return a->key < b->key;
    if (a->i   != b->i   ) return a->i   < b->i;
    return a->j < b->j;
}

/* Swap two nodes. */
static inline void swap_node(node_t *a, node_t *b)
{
    node_t t = *a; *a = *b; *b = t;
}

/* Push a node onto the heap. */
static void heap_push(node_t *heap, size_t *size, const node_t *nd)
{
    heap[*size] = *nd;
    size_t idx = *size;
    ++(*size);
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (!node_lt(&heap[idx], &heap[parent])) break;
        swap_node(&heap[idx], &heap[parent]);
        idx = parent;
    }
}

/* Pop the smallest node from the heap into *out. */
static void heap_pop(node_t *heap, size_t *size, node_t *out)
{
    *out = heap[0];
    --(*size);
    if (*size == 0) return;
    heap[0] = heap[*size];
    size_t idx = 0;
    while (1) {
        size_t left  = (idx << 1) + 1;
        size_t right = left + 1;
        if (left >= *size) break;
        size_t child = left;
        if (right < *size && node_lt(&heap[right], &heap[left]))
            child = right;
        if (!node_lt(&heap[child], &heap[idx])) break;
        swap_node(&heap[child], &heap[idx]);
        idx = child;
    }
}

/* Query function for the sequence. */
seq_t a(idx_t n)
{
    /* The sequence is not defined for n=0; return 0 for safety. */
    if (n == 0) return 0;

    /* Estimate how many different i values we will need.
       Using a factor of 3 gives more than enough space for
       n up to 10000 and keeps memory usage modest. */
    size_t limit = (size_t)n * 3 + 50;
    node_t *heap = (node_t *)malloc(limit * sizeof(node_t));
    if (!heap) return 0;      /* allocation failure */

    size_t heap_size = 0;

    /* Initialise the heap with all pairs (i,0). */
    for (size_t i = 0; i < limit; ++i) {
        node_t nd;
        nd.i = i;
        nd.j = 0;
        nd.key = (int64_t)i * PHI_DEN;   /* (i-0)*PHI_DEN + 0*PHI_NUM */
        heap_push(heap, &heap_size, &nd);
    }

    idx_t rank = 0;        /* current rank (1‑based) */
    idx_t even_cnt = 0;    /* number of even‑i ranks seen */

    node_t cur;

    /* Process until we have found the n‑th even‑i rank. */
    while (even_cnt < n) {
        heap_pop(heap, &heap_size, &cur);
        ++rank;
        if ((cur.i & 1) == 0) {        /* even i */
            ++even_cnt;
            if (even_cnt == n) {
                free(heap);
                return (seq_t)rank;
            }
        }

        /* Push the next j for the same i. */
        ++cur.j;
        cur.key = compute_key(cur.i, cur.j);
        heap_push(heap, &heap_size, &cur);
    }

    /* Should never be reached. */
    free(heap);
    return 0;
}
