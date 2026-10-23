/*
 * A124934  Numbers of the form 4mn - m - n, where m, n are positive integers.
 * https://oeis.org/A124934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   formula
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A124934: a(n) is the n‑th smallest integer of the form
   4*m*n - m - n with m,n ≥ 1.
   The sequence is generated on demand by expanding each
   stream m=1,2,3,… and repeatedly extracting the smallest
   available value using a binary min‑heap.
   Complexity is O(n log n). */

#include <stdlib.h>
#include "../utils/formula.h"

/* ------------------------------------------------------------------ */
/* Heap node -------------------------------------------------------- */
typedef struct {
    seq_t val;   /* 4*m*n - m - n */
    idx_t m;     /* stream index (m) */
    idx_t n;     /* position in the stream (n) */
} heap_node_t;

/* ------------------------------------------------------------------ */
/* Binary min‑heap helpers ------------------------------------------- */
static heap_node_t *heap = NULL;
static size_t heap_cap = 0;
static size_t heap_size = 0;

/* Swap two heap entries */
static void heap_swap(heap_node_t *a, heap_node_t *b)
{
    heap_node_t tmp = *a;
    *a = *b;
    *b = tmp;
}

/* Push a node onto the heap */
static void heap_push(heap_node_t node)
{
    if (heap_size >= heap_cap) {
        size_t new_cap = heap_cap ? heap_cap << 1 : 8;
        heap = (heap_node_t *)realloc(heap, new_cap * sizeof(heap_node_t));
        heap_cap = new_cap;
    }
    size_t i = heap_size++;
    heap[i] = node;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[i].val >= heap[p].val) break;
        heap_swap(&heap[i], &heap[p]);
        i = p;
    }
}

/* Pop the smallest node from the heap */
static heap_node_t heap_pop(void)
{
    heap_node_t min = heap[0];
    heap[0] = heap[--heap_size];
    size_t i = 0;
    while (1) {
        size_t l = i * 2 + 1, r = l + 1, smallest = i;
        if (l < heap_size && heap[l].val < heap[smallest].val) smallest = l;
        if (r < heap_size && heap[r].val < heap[smallest].val) smallest = r;
        if (smallest == i) break;
        heap_swap(&heap[i], &heap[smallest]);
        i = smallest;
    }
    return min;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Memoised cache of the sequence ----------------------------------- */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_size = 0;      /* number of terms stored */

/* Grow the cache to accommodate at least 'needed' elements */
static void grow_cache(idx_t needed)
{
    if (cache_cap < (size_t)needed) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < (size_t)needed) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/* Manage the streams ----------------------------------------------- */
static idx_t next_m = 1;          /* next stream to seed */

/* Seed the first element of stream m */
static void seed_stream(idx_t m)
{
    heap_node_t node;
    node.val = formula(m, (idx_t)1);
    node.m   = m;
    node.n   = (idx_t)1;
    heap_push(node);
}

/* ------------------------------------------------------------------ */
/* Main function: a(n) ---------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* offset 1 */

    /* Produce terms until we have enough */
    while (cache_size < n) {
        /* Seed all streams whose first value is <= current minimum */
        while (1) {
            seq_t next_first = formula(next_m, (idx_t)1);
            if (heap_size == 0 || next_first <= heap[0].val) {
                seed_stream(next_m);
                next_m++;
            } else {
                break;
            }
        }

        /* Extract next smallest value */
        heap_node_t node = heap_pop();
        seq_t val = node.val;

        /* Store distinct values only */
        if (cache_size == 0 || val != cache[cache_size - 1]) {
            grow_cache(cache_size + 1);
            cache[cache_size++] = val;
        }

        /* Push the successor of this node into its stream */
        idx_t m  = node.m;
        idx_t nn = node.n + (idx_t)1;
        heap_push((heap_node_t){ formula(m, nn), m, nn });
    }

    return cache[n - 1];
}
