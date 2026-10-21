/*
 * A088247  Orders of proper semifields.
 * https://oeis.org/A088247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = sorted list of all prime powers p^k with p prime, k >= 3,
 *            and p^k >= 16
 * Range:     u8 none, u16 none, u32 n <= 403, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   big_pow, build_primes_upto, cmp_seq_t, seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation lazily builds the sequence using a min‑heap of the
    smallest power from each prime.  Each time we pop the minimal power,
    push the next higher power of the same prime back into the heap.
    The array `cache` stores the sequence up to the largest n requested. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/cmp_seq_t.h"
#include "../utils/seq_realloc.h"
#include "../utils/build_primes_upto.h"

typedef struct HeapNode {
    seq_t  val;     /* current prime power */
    uint64_t prime;/* base prime           */
    uint32_t exp;   /* current exponent    */
} HeapNode;

static inline int heap_node_less(const HeapNode *a, const HeapNode *b)
{
    return cmp_seq_t(&(a->val), &(b->val)) < 0;
}

static inline void heap_swap(HeapNode *x, HeapNode *y)
{
    HeapNode tmp = *x;
    *x = *y;
    *y = tmp;
}

static void heap_heapify_down(HeapNode *h, size_t sz, size_t idx)
{
    while (true) {
        size_t l = 2 * idx + 1;
        size_t r = 2 * idx + 2;
        size_t smallest = idx;

        if (l < sz && heap_node_less(&h[l], &h[smallest]))
            smallest = l;
        if (r < sz && heap_node_less(&h[r], &h[smallest]))
            smallest = r;

        if (smallest != idx) {
            heap_swap(&h[smallest], &h[idx]);
            idx = smallest;
        } else
            break;
    }
}

static void heap_heapify_up(HeapNode *h, size_t sz, size_t idx)
{
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap_node_less(&h[idx], &h[parent])) {
            heap_swap(&h[idx], &h[parent]);
            idx = parent;
        } else
            break;
    }
}

static void heap_build(HeapNode *h, size_t sz)
{
    if (sz <= 1) return;
    for (size_t i = (sz >> 1); i > 0; --i)
        heap_heapify_down(h, sz, i - 1);
}

static void heap_push(HeapNode *h, size_t *sz, HeapNode node)
{
    h[*sz] = node;
    heap_heapify_up(h, *sz + 1, *sz);
    (*sz)++;
}

static HeapNode heap_pop(HeapNode *h, size_t *sz)
{
    HeapNode ret = h[0];
    h[0] = h[*sz - 1];
    (*sz)--;
    heap_heapify_down(h, *sz, 0);
    return ret;
}

seq_t a(idx_t n)
{
    /* ======== static state (lazy initialized) ======== */
    static bool initialized = false;
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static HeapNode *heap = NULL;
    static size_t heap_size = 0;

    /* ----- initialization ------------------------------------ */
    if (!initialized) {
        /* Prime limit chosen large enough to generate ≥10 000 terms */
        const idx_t prime_limit = 200000ULL;

        build_primes_upto(prime_limit, &primes, &primes_cnt);

        heap = (HeapNode *)malloc(primes_cnt * sizeof(HeapNode));
        if (!heap) exit(EXIT_FAILURE);

        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            uint32_t start_exp = (p == 2 ? 4 : 3);  /* 2³=8 is too small */
            HeapNode node;
            node.prime = p;
            node.exp   = start_exp;
            node.val   = big_pow((seq_t)p, (seq_t)start_exp);
            heap[i] = node;
        }

        heap_build(heap, primes_cnt);
        heap_size = primes_cnt;

        cache_len = 0;
        cache_cap = 0;
        cache = NULL;

        initialized = true;
    }

    /* ----- generate terms until requested index reached -------- */
    while (cache_len < n) {
        /* grow cache if necessary */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : (cache_cap << 1);
            seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
            if (!tmp) exit(EXIT_FAILURE);
            cache     = tmp;
            cache_cap = new_cap;
        }

        /* smallest power available */
        HeapNode node = heap_pop(heap, &heap_size);

        /* store in cache */
        cache[cache_len++] = node.val;

        /* advance this prime to its next power */
        node.val = seq_mul(node.val, (seq_t)node.prime);
        node.exp += 1;

        /* re‑insert */
        heap_push(heap, &heap_size, node);
    }

    return cache[n - 1];
}
