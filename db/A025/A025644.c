/*
 * A025644  Exponent of 3 (value of i) in n-th number of form 3^i*10^j.
 * https://oeis.org/A025644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A025644 – Exponent of 3 in the n‑th number of the form 3^i*10^j
 *
 *  The sequence consists of the exponents i for the numbers
 *  3^i * 10^j, j,i >= 0, listed in increasing order.
 *  The function builds this sorted list once with a
 *  min‑priority queue and returns the i –‑th term on demand.
 *
 *  Complexity: O(n log k) per term, with k being the number of
 *  active series (≈ log_10(n)).  For n ≤ 10 000 this easily
 *  fits within the 2 second limit.
 *
 *  The implementation uses only integer arithmetic on the cache and
 *  a long‑double key for comparing the terms:
 *       key(i,j) = j + i * log10(3)
 */

#include <stdlib.h>   /* malloc, realloc, free */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>     /* isnan on some systems */

static const long double LOG10_3 = 0.477121254719L;

/* Node describing a number 3^i * 10^j */
typedef struct {
    idx_t i;   /* exponent of 3   */
    idx_t j;   /* exponent of 10  */
} node_t;

/* ----------  Min‑heap of nodes ---------- */
static node_t *heap = NULL;     /* heap array                */
static size_t  heap_size = 0;   /* number of elements stored */
static size_t  heap_cap  = 0;   /* allocated capacity        */

/* Compute a sortable key for a node.  Using long double gives
 * more precision than float and is still fast on all target
 * architectures.  The key is monotone with the numeric value
 * 3^i * 10^j. */
static inline long double node_key(const node_t *n)
{
    return (long double)n->j + (long double)n->i * LOG10_3;
}

static inline void heap_swap(node_t *a, node_t *b)
{
    node_t tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Push a node into the min‑heap */
static void heap_push(node_t n)
{
    if (heap_size >= heap_cap) {
        size_t new_cap = heap_cap ? heap_cap * 2 : 32;
        heap = (node_t *)realloc(heap, new_cap * sizeof(node_t));
        if (!heap) __builtin_trap();
        heap_cap = new_cap;
    }

    size_t pos = heap_size++;
    heap[pos] = n;

    /* percolate up */
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (node_key(&heap[pos]) >= node_key(&heap[parent]))
            break;
        heap_swap(&heap[pos], &heap[parent]);
        pos = parent;
    }
}

/* Pop the smallest node from the heap; the result is stored
 * in *out.  The heap must be non‑empty. */
static void heap_pop(node_t *out)
{
    *out = heap[0];
    heap[0] = heap[--heap_size];

    /* percolate down */
    size_t pos = 0;
    while (1) {
        size_t left  = (pos << 1) + 1;
        size_t right = left + 1;
        size_t smallest = pos;

        if (left  < heap_size && node_key(&heap[left])  < node_key(&heap[smallest]))
            smallest = left;
        if (right < heap_size && node_key(&heap[right]) < node_key(&heap[smallest]))
            smallest = right;

        if (smallest == pos)
            break;
        heap_swap(&heap[pos], &heap[smallest]);
        pos = smallest;
    }
}

/* ----------  Cache of computed sequence values ---------- */
static seq_t  *seq_cache  = NULL;
static size_t  seq_cap    = 0;
static size_t  seq_len    = 0;   /* number of terms already cached */

/* ----------  Static state shared across calls ---------- */
static bool    initialized = false;
static idx_t   next_j_to_add = 1;  /* next j to start a new series */

/* ----------  Public routine  ---------- */
seq_t a(idx_t n)
{
    /* Undefined for n == 0 – return 0 safely. */
    if (n == 0)
        return 0;

    /* First time: allocate cache, initialise heap and first series. */
    if (!initialized) {
        initialized = true;

        seq_cap    = 64;
        seq_cache  = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq_cache) __builtin_trap();

        heap_cap = 32;
        heap     = (node_t *)malloc(heap_cap * sizeof(node_t));
        if (!heap) __builtin_trap();

        /* Start with the series j = 0 : 1, 3, 9, 27, ... */
        heap_push((node_t){0, 0});
        /* next_j_to_add is already 1 */
    }

    /* Grow the cache until we have at least n values. */
    while (seq_len < (size_t)n) {
        /* 1.  Pop the current smallest term. */
        node_t cur;
        heap_pop(&cur);

        /* 2.  Store its i‑exponent as the next sequence value. */
        if (seq_len >= seq_cap) {
            size_t new_cap = seq_cap * 2;
            seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            if (!seq_cache) __builtin_trap();
            seq_cap = new_cap;
        }
        seq_cache[seq_len++] = (seq_t)cur.i;

        /* 3.  Push the next element of this series (i+1 , j). */
        heap_push((node_t){cur.i + 1, cur.j});

        /* 4.  Lazily add new series starting at j = next_j_to_add
         *     whenever the smallest number still in the heap is already
         *     greater than 10^next_j.  This guarantees we keep all
         *     numbers sorted without scanning all j up to the current limit.
         */
        while (true) {
            long double current_min = node_key(&heap[0]);       /* key of smallest remaining number */
            long double start_key   = (long double)next_j_to_add; /* key of 10^next_j  */
            if (start_key <= current_min) {
                heap_push((node_t){0, next_j_to_add});
                ++next_j_to_add;
            } else {
                break;
            }
        }
    }

    return seq_cache[n - 1];
}
