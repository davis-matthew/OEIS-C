/*
 * A176406  Odd semiprimes minus 2.
 * https://oeis.org/A176406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A046315(n) - 2 (odd semiprimes minus 2)
 * Range:     u8 none, u16 none, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes.h"

/* Node in the min‑heap of products of two odd primes. */
typedef struct {
    seq_t val;          /* product p_i * p_j          */
    size_t i;          /* index of the first prime   */
    size_t j;          /* index of the second prime  */
} HeapNode;

/* Cached values of a(n) */
static seq_t  *cache      = NULL;   /* array of a(1), a(2), ...     */
static size_t cache_len  = 0;       /* number of values in cache    */
static size_t cache_cap  = 0;       /* capacity of cache array      */

/* List of odd primes up to the current limit */
static uint64_t *odd_primes = NULL; /* odd primes: 3,5,7,...         */
static size_t   odd_cnt    = 0;     /* number of odd primes         */

/* Min‑heap of candidate products (persistent across calls) */
static HeapNode *heap      = NULL;  /* array of heap nodes          */
static size_t   heap_cap   = 0;     /* capacity of heap array       */
static size_t   heap_size  = 0;     /* current number of nodes      */

/* Forward declarations of heap helpers */
static void heap_sift_down(HeapNode *h, size_t sz, size_t idx);
static void heap_sift_up   (HeapNode *h, size_t idx);
static HeapNode heap_pop   (HeapNode *h, size_t *sz);

/* --- Heap manipulation --------------------------------------------------- */
static void heap_sift_down(HeapNode *h, size_t sz, size_t idx)
{
    while (true) {
        size_t left  = 2 * idx + 1;
        size_t right = left + 1;
        size_t smallest = idx;

        if (left  < sz && h[left].val  < h[smallest].val) smallest = left;
        if (right < sz && h[right].val < h[smallest].val) smallest = right;

        if (smallest == idx) break;

        HeapNode tmp = h[idx];
        h[idx] = h[smallest];
        h[smallest] = tmp;
        idx = smallest;
    }
}

static void heap_sift_up(HeapNode *h, size_t idx)
{
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (h[parent].val <= h[idx].val) break;
        HeapNode tmp = h[parent];
        h[parent] = h[idx];
        h[idx] = tmp;
        idx = parent;
    }
}

static HeapNode heap_pop(HeapNode *h, size_t *sz)
{
    HeapNode min = h[0];
    h[0] = h[*sz - 1];
    (*sz)--;
    heap_sift_down(h, *sz, 0);
    return min;
}

/* ------------------------------------------------------------------------ */
/* Ensure that at least n terms are available in cache.  Consecutive calls
   are handled efficiently by reusing the heap and cache. */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n) return;    /* already ready */

    /* 1. Build odd primes if not yet done */
    if (odd_primes == NULL) {
        /* Sufficient prime limit for n up to ~10000 */
        const idx_t limit = 400000U;

        idx_t *prs = NULL;
        size_t cnt = 0;
        build_primes(limit, &prs, &cnt);   /* returns all primes up to limit */

        /* Record odd primes (skip the prime 2) */
        odd_cnt    = cnt - 1;               /* exclude 2 */
        odd_primes = (uint64_t*)malloc(sizeof(uint64_t) * odd_cnt);
        for (size_t k = 0; k < odd_cnt; ++k)
            odd_primes[k] = (uint64_t)prs[k + 1];  /* prs[0] == 2 */

        free(prs);

        /* 2. Initialise the min‑heap once */
        heap_cap = odd_cnt;
        heap     = (HeapNode*)malloc(sizeof(HeapNode) * heap_cap);
        heap_size = odd_cnt;
        for (size_t i = 0; i < odd_cnt; ++i) {
            heap[i].i   = i;
            heap[i].j   = i;
            heap[i].val = (seq_t)odd_primes[i] * (seq_t)odd_primes[i];
        }
        /* Build heap property */
        for (size_t i = heap_size / 2; i-- > 0; )
            heap_sift_down(heap, heap_size, i);
    }

    /* 3. Generate terms until cache_len >= n */
    while (cache_len < (size_t)n) {
        HeapNode node = heap_pop(heap, &heap_size);

        /* Store the value minus 2 */
        seq_t val_minus_2 = node.val - (seq_t)2;
        if (cache_len >= cache_cap) {
            cache_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
            cache = (seq_t*)realloc(cache, sizeof(seq_t) * cache_cap);
        }
        cache[cache_len++] = val_minus_2;

        /* Push the next product with the same first prime */
        if (node.j + 1 < odd_cnt) {
            size_t new_i = node.i;
            size_t new_j = node.j + 1;
            heap[heap_size].i   = new_i;
            heap[heap_size].j   = new_j;
            heap[heap_size].val = (seq_t)odd_primes[new_i] *
                                  (seq_t)odd_primes[new_j];
            heap_size++;
            heap_sift_up(heap, heap_size - 1);
        }
    }
}

/* ------------------------------------------------------------------------ */
/* Public function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; return 0 for n==0 to avoid UB */
        return (seq_t)0;
    ensure_cache(n);
    return cache[n - 1];
}
