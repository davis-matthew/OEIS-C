/*
 * A234000  Numbers of the form 4^i*(8*j+1).
 * https://oeis.org/A234000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = numbers of the form 4^i · (8 j + 1) in increasing order
 *            (OEIS A234000)
 * Range:     u8 n <= 12, u16 n <= 2976, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* --------------------------------------------------------------------- */
/* Helper data structures                                                */
/* --------------------------------------------------------------------- */
typedef struct {
    seq_t val;     /* current value for this i   */
    idx_t i;       /* index i (i.e. 0,1,2,…)    */
} node_t;

/* --------------------------------------------------------------------- */
/* Static state for memoization and incremental generation                */
/* --------------------------------------------------------------------- */
static seq_t *pow4 = NULL;     /* 4^i     */
static seq_t *inc  = NULL;     /* 8·4^i   */
static idx_t  pow4_len = 0;    /* number of powers generated */

/* Cache for already computed sequence values */
static seq_t *cache        = NULL;
static idx_t  cache_cap    = 0;
static idx_t  cache_len    = 0;

/* Min‑heap of next candidate values for each i */
static node_t *heap        = NULL;
static size_t  heap_cap    = 0;
static size_t  heap_len    = 0;

/* --------------------------------------------------------------------- */
/* Utility functions                                                    */
/* --------------------------------------------------------------------- */
static void ensure_cache_cap(idx_t needed)
{
    if (needed > cache_cap) {
        size_t newcap = cache_cap ? cache_cap * 2 : 64;
        while (newcap < (size_t)needed) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = newcap;
    }
}

static void ensure_heap_cap(size_t needed)
{
    if (needed > heap_cap) {
        size_t newcap = heap_cap ? heap_cap * 2 : 8;
        while (newcap < needed) newcap *= 2;
        node_t *tmp = (node_t *)realloc(heap, newcap * sizeof(node_t));
        if (!tmp) abort();
        heap = tmp;
        heap_cap = newcap;
    }
}

/* --------------------------------------------------------------------- */
/* Heap operations (min‑heap on node.val)                                */
/* --------------------------------------------------------------------- */
static void heap_push(node_t n)
{
    ensure_heap_cap(heap_len + 1);
    size_t idx = heap_len++;
    heap[idx] = n;
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap[parent].val <= heap[idx].val) break;
        node_t tmp = heap[parent];
        heap[parent] = heap[idx];
        heap[idx] = tmp;
        idx = parent;
    }
}

static node_t heap_pop(void)
{
    node_t result = heap[0];
    heap[0] = heap[--heap_len];
    size_t idx = 0;
    while (1) {
        size_t left = (idx << 1) + 1;
        if (left >= heap_len) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < heap_len && heap[right].val < heap[left].val)
            smallest = right;
        if (heap[smallest].val >= heap[idx].val) break;
        node_t tmp = heap[smallest];
        heap[smallest] = heap[idx];
        heap[idx] = tmp;
        idx = smallest;
    }
    return result;
}

/* --------------------------------------------------------------------- */
/* Ensure powers of 4 up to at least 6·n are available                  */
/* Every time a new power is added it is pushed into the heap.           */
/* --------------------------------------------------------------------- */
static void extend_powers(idx_t n)
{
    seq_t limit = (seq_t)6 * (seq_t)n;          /* 6*n is enough for n terms */
    if (pow4_len == 0) {
        /* initial power of 4 */
        pow4 = (seq_t *)malloc(1 * sizeof(seq_t));
        inc  = (seq_t *)malloc(1 * sizeof(seq_t));
        pow4_len = 1;
        pow4[0] = (seq_t)1;
        inc[0]  = (seq_t)8;
        heap_push((node_t){pow4[0], 0});
        return;
    }

    /* compute next powers until we exceed the limit */
    while (pow4[pow4_len-1] <= limit / 4) {
        /* grow arrays */
        seq_t *tpow4 = (seq_t *)realloc(pow4, (pow4_len + 1) * sizeof(seq_t));
        seq_t *tinc  = (seq_t *)realloc(inc,  (pow4_len + 1) * sizeof(seq_t));
        if (!tpow4 || !tinc) abort();
        pow4 = tpow4;
        inc  = tinc;
        pow4[pow4_len] = pow4[pow4_len-1] * (seq_t)4;
        inc[pow4_len]  = pow4[pow4_len]  * (seq_t)8;
        /* push new initial value into heap */
        heap_push((node_t){pow4[pow4_len], (idx_t)pow4_len});
        pow4_len++;
    }
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                                */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)   /* safety guard; offset is 1 */
        return 0;

    if (n > cache_len) {
        /* make sure we have enough powers of 4 */
        extend_powers(n);

        /* extend cache up to n */
        ensure_cache_cap((idx_t)(n + 1));   /* +1 for safety */
        while (cache_len < n) {
            node_t cur = heap_pop();
            cache[cache_len++] = cur.val;
            /* advance this i by its increment */
            cur.val += inc[cur.i];
            heap_push(cur);
        }
    }
    return cache[n-1];
}
