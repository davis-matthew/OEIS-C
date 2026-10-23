/*
 * A195758  Lesser prime factor of n-th Blum number.
 * https://oeis.org/A195758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 7387, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A195758 – Lesser prime factor of the n-th Blum number.
 *
 *  Formula: a(n) is the smallest prime factor of the n‑th smallest
 *  integer that is the product of two distinct primes both ≡ 3 (mod 4).
 *
 *  The algorithm generates all primes ≡ 3 (mod 4), then uses a
 *  min‑heap of candidate products p[i] * p[j] (i < j) to emit Blum
 *  numbers in increasing order.  For each candidate that becomes
 *  minimal we output p[i] (the smaller prime factor).  The algorithm
 *  memoises all results in a static dynamic array so subsequent
 *  calls are O(1) after the required value has been computed.
 *
 *  The implementation works for any seq_t and idx_t that the build
 *  supplies – it never redefines seq_t, and only relies on 64‑bit
 *  integers for intermediate products.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"   /* Provides build_primes_upto */

/* ------------------------------------------------------------------ */
/*  Data structures                                                    */
/* ------------------------------------------------------------------ */

typedef struct {
    uint64_t prod;   /* p[i] * p[j] – used only for ordering          */
    size_t   i;      /* index of the smaller prime (p[i])            */
    size_t   j;      /* index of the larger prime (p[j])             */
} heap_node;

/* ------------------------------------------------------------------ */
/*  Global state (static, one‑time initialization)                    */
/* ------------------------------------------------------------------ */

static uint64_t *primes3      = NULL;   /* primes ≡ 3 (mod 4)                      */
static size_t   primes3_cnt   = 0;      /* number of such primes                   */

static heap_node *heap       = NULL;   /* min‑heap of next Blum candidates       */
static size_t    heap_len    = 0;      /* current heap size                       */

static seq_t *blum_cache    = NULL;   /* memoised a(n) values                    */
static size_t blum_len      = 0;      /* number of values cached                 */
static size_t blum_cap      = 0;      /* capacity of blum_cache                   */

static int initialized     = 0;       /* true after first use                    */

/* ------------------------------------------------------------------ */
/*  Heap helpers                                                       */
/* ------------------------------------------------------------------ */

static void sift_up(size_t pos) {
    heap_node val = heap[pos];
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (heap[parent].prod <= val.prod) break;
        heap[pos] = heap[parent];
        pos = parent;
    }
    heap[pos] = val;
}

static void sift_down(size_t pos) {
    heap_node val = heap[pos];
    size_t half = heap_len >> 1;
    while (pos < half) {
        size_t left  = (pos << 1) + 1;
        size_t right = left + 1;
        size_t child = (right < heap_len && heap[right].prod < heap[left].prod)
                        ? right : left;
        if (heap[child].prod >= val.prod) break;
        heap[pos] = heap[child];
        pos = child;
    }
    heap[pos] = val;
}

/* ------------------------------------------------------------------ */
/*  Initialization (prime generation, heap construction, cache)      */
/* ------------------------------------------------------------------ */

static void init_sequence(void) {
    /* A safe upper bound for generating enough primes ≡3 (mod 4).
       For n≈10000 this bound is more than sufficient.                */
    const idx_t prime_limit = 200000;

    uint64_t *pr_all  = NULL;
    size_t    cnt_all = 0;
    build_primes_upto(prime_limit, &pr_all, &cnt_all);

    primes3 = (uint64_t *)malloc(cnt_all * sizeof(uint64_t));
    for (size_t k = 0; k < cnt_all; ++k)
        if (pr_all[k] % 4 == 3)
            primes3[primes3_cnt++] = pr_all[k];
    free(pr_all);

    /* Heap contains one candidate per i (i < primes3_cnt-1).       */
    heap_len = (primes3_cnt > 0) ? primes3_cnt - 1 : 0;
    heap = (heap_node *)malloc(heap_len * sizeof(heap_node));
    for (size_t i = 0; i < heap_len; ++i) {
        heap[i].i = i;
        heap[i].j = i + 1;
        heap[i].prod = primes3[i] * primes3[i + 1];
    }
    for (size_t k = (heap_len >> 1); k > 0; )
        sift_down(--k);

    /* Cache for a(n) values – start with capacity 1024.            */
    blum_cap  = (blum_len > 0) ? 1024 : 0;
    blum_cache = (seq_t *)malloc(blum_cap * sizeof(seq_t));

    initialized = 1;
}

/* ------------------------------------------------------------------ */
/*  Generate next Blum number, storing its smaller prime factor.      */
/* ------------------------------------------------------------------ */

static void generate_next(void) {
    if (heap_len == 0) return;   /* should not happen with ample primes */

    heap_node cur = heap[0];
    blum_cache[blum_len++] = (seq_t)primes3[cur.i];

    /* Advance (i, j) to the next larger prime for the same i.     */
    if (cur.j + 1 < primes3_cnt) {
        heap[0].j = cur.j + 1;
        heap[0].prod = primes3[cur.i] * primes3[cur.j + 1];
        sift_down(0);
    } else {
        /* i is exhausted – replace root with last node.            */
        heap[0] = heap[heap_len - 1];
        heap_len--;
        if (heap_len > 0)
            sift_down(0);
    }

    /* Grow cache if necessary.                                      */
    if (blum_len == blum_cap) {
        blum_cap <<= 1;
        blum_cache = (seq_t *)realloc(blum_cache, blum_cap * sizeof(seq_t));
    }
}

/* ------------------------------------------------------------------ */
/*  Public function                                                     */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined, return 0 */

    if (!initialized) init_sequence();

    /* Compute values up to the requested index.                     */
    while (blum_len < n) generate_next();

    return blum_cache[n - 1];
}
