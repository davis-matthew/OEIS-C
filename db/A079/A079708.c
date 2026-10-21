/*
 * A079708  Metaprime binary to standard binary conversion series.
 * https://oeis.org/A079708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0 a(n) = A052330(a(n-1)) for n > 0 where A052330(x) = ∏
 *            m(k) for k such that the k‑th bit of x is 1, and m(k) runs
 *            through the sequence of metaprimes p^(2^e) in increasing
 *            numerical order
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 15, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, build_primes_upto, cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/cmp_seq_t.h"
#include "../utils/big_pow.h"
#include "../utils/build_primes_upto.h"

/* ---- global state ----------------------------------------------------- */
static seq_t *cache = NULL;               /* memoised a(n) values */
static size_t cache_cap = 0;
static idx_t last_cached = 0;             /* largest n computed so far */

static uint64_t *prime_array = NULL;      /* list of primes for metaprime base */
static size_t prime_count = 0;

struct HeapNode {
    seq_t  val;        /* current metaprime value   */
    uint64_t prime;    /* base prime                */
    idx_t   exp;       /* current exponent (1,2,4,..) */
};

static struct HeapNode *heap = NULL;
static size_t heap_cap = 0, heap_size = 0;

static seq_t *meta_list = NULL;           /* list of metaprimes m(k)   */
static size_t meta_cap = 0, meta_count = 0;

/* ---- helper: heap ordering ------------------------------------------- */
static inline int seq_lt(seq_t a, seq_t b) {
    return cmp_seq_t(&a, &b) < 0;
}

static void heap_swap(struct HeapNode *a, struct HeapNode *b) {
    struct HeapNode tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heap_heapify_up(size_t idx) {
    if (idx == 0) return;
    size_t parent = (idx - 1) >> 1;
    if (!seq_lt(heap[idx].val, heap[parent].val)) return;
    heap_swap(&heap[idx], &heap[parent]);
    heap_heapify_up(parent);
}

static void heap_heapify_down(size_t idx) {
    while (1) {
        size_t left = (idx << 1) + 1;
        size_t right = left + 1;
        size_t smallest = idx;
        if (left < heap_size && seq_lt(heap[left].val, heap[smallest].val))
            smallest = left;
        if (right < heap_size && seq_lt(heap[right].val, heap[smallest].val))
            smallest = right;
        if (smallest == idx) break;
        heap_swap(&heap[idx], &heap[smallest]);
        idx = smallest;
    }
}

static void heap_push(struct HeapNode node) {
    if (heap_size == heap_cap) {
        heap_cap = heap_cap ? heap_cap << 1 : 16;
        heap = (struct HeapNode *)realloc(heap, heap_cap * sizeof(struct HeapNode));
        if (!heap) abort();
    }
    heap[heap_size++] = node;
    heap_heapify_up(heap_size - 1);
}

static struct HeapNode heap_pop(void) {
    struct HeapNode min = heap[0];
    heap[0] = heap[--heap_size];
    heap_heapify_down(0);
    return min;
}

/* ---- initialise primes and heap ------------------------------------- */
static void meta_init(void) {
    if (prime_array) return;          /* already initialised */

    const idx_t PRIME_LIM = 200000;   /* sufficient for n up to ~10 000 */
    build_primes_upto(PRIME_LIM, &prime_array, &prime_count);

    heap_cap = prime_count;
    heap     = (struct HeapNode *)malloc(heap_cap * sizeof(struct HeapNode));
    heap_size = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        struct HeapNode node;
        node.prime = prime_array[i];
        node.exp   = 1;
        node.val   = (seq_t)prime_array[i];
        heap_push(node);
    }

    meta_cap   = 256;
    meta_list  = (seq_t *)malloc(meta_cap * sizeof(seq_t));
    meta_count = 0;
}

/* ---- make sure we have metaprimes up to index idx_needed ------------- */
static void ensure_metaprime(idx_t idx_needed) {
    meta_init();
    while (meta_count <= idx_needed) {
        struct HeapNode node = heap_pop();

        if (meta_count == meta_cap) {
            meta_cap <<= 1;
            meta_list = (seq_t *)realloc(meta_list, meta_cap * sizeof(seq_t));
            if (!meta_list) abort();
        }
        meta_list[meta_count++] = node.val;

        node.exp <<= 1;                                 /* next power */
        node.val = big_pow((seq_t)node.prime,
                           (seq_t)node.exp);
        heap_push(node);
    }
}

/* ---- A052330: product of metaprimes for set bits of x ---------------- */
static seq_t A052330(seq_t x) {
    seq_t result = 1;
    idx_t pos = 0;
    while (x > 0) {
        if (x & 1) {
            if ((size_t)pos >= meta_count) ensure_metaprime(pos);
            result *= meta_list[(size_t)pos];
        }
        ++pos;
        x >>= 1;
    }
    return result;
}

/* ---- main sequence function ------------------------------------------ */
seq_t a(idx_t n) {
    if (cache == NULL) {                      /* first call */
        cache     = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache_cap = 1;
        cache[0]  = 0;
        last_cached = 0;
        meta_init();
    }

    if (n >= cache_cap) {                    /* grow cache if needed */
        size_t new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) abort();
        cache_cap = new_cap;
    }

    /* compute any missing terms up to n */
    for (idx_t i = last_cached + 1; i <= n; ++i) {
        cache[i] = A052330(cache[i - 1]);
    }
    if (n > last_cached) last_cached = n;

    return cache[n];
}
