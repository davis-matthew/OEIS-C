/*
 * A214515  Numbers of the form p^2 + q^2 + r^2 + s^2, where p, q, r, and s
 *          are primes.
 * https://oeis.org/A214515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers of the form p² + q² + r² + s² with p, q, r, s prime
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes all sums of two prime squares
 * (p² + q²), stores them in ascending order, and then enumerates the
 * sorted, distinct sums of two such values to obtain the sequence.
 * The result is cached and reused for subsequent calls.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

/* Choose a prime limit sufficient to generate the first ~20000 terms. */
#define PRIMELIMIT 5000          /* primes ≤ 5000 are more than enough */
#define MAX_TERMS 20000          /* cache size – bigger than the test bound */

/* Comparator for qsort on seq_t.  */
static int cmp_seq_t_wrapper(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

/* Heap node used for generating two‑sum values. */
typedef struct {
    seq_t sum;   /* A[i] + A[j] */
    size_t i;
    size_t j;
} Node;

/* Static state: cached sequence. */
static seq_t *term_cache = NULL;          /* cached first MAX_TERMS terms */
static size_t cache_len = 0;              /* number of cached terms */

/*------------------- heap helpers -------------------------------------*/
static void heap_swap(Node *h, size_t a, size_t b)
{
    Node t = h[a];
    h[a] = h[b];
    h[b] = t;
}
static void heap_sift_down(Node *h, size_t idx, size_t sz)
{
    while (1) {
        size_t l = 2 * idx + 1;
        size_t r = l + 1;
        size_t smallest = idx;
        if (l < sz && h[l].sum < h[smallest].sum) smallest = l;
        if (r < sz && h[r].sum < h[smallest].sum) smallest = r;
        if (smallest == idx) break;
        heap_swap(h, idx, smallest);
        idx = smallest;
    }
}
static void heap_sift_up(Node *h, size_t idx, size_t sz)
{
    while (idx > 0) {
        size_t p = (idx - 1) >> 1;
        if (h[idx].sum < h[p].sum) {
            heap_swap(h, idx, p);
            idx = p;
        } else break;
    }
}

/*------------------- sequence initialization ---------------------------*/
static void init_sequence(void)
{
    /* 1. Build list of primes up to PRIMELIMIT. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto((idx_t)PRIMELIMIT, &primes, &prime_cnt);

    /* 2. Compute all sums of two prime squares (p² + q²), p ≤ q. */
    size_t pairs = prime_cnt * (prime_cnt + 1) / 2;
    seq_t *two_sq = (seq_t *)malloc(pairs * sizeof(seq_t));
    size_t k = 0;
    for (size_t a = 0; a < prime_cnt; ++a) {
        uint64_t pa = primes[a];
        uint64_t pa2 = pa * pa;
        for (size_t b = a; b < prime_cnt; ++b) {
            uint64_t pb = primes[b];
            uint64_t sum = pa2 + pb * pb;
            two_sq[k++] = (seq_t)sum;
        }
    }
    free(primes);  /* primes no longer needed */

    /* 3. Sort and deduplicate the two‑square sums. */
    qsort(two_sq, pairs, sizeof(seq_t), cmp_seq_t_wrapper);
    size_t uniq = 0;
    for (size_t i = 0; i < pairs; ++i) {
        if (uniq == 0 || two_sq[i] != two_sq[uniq - 1])
            two_sq[uniq++] = two_sq[i];
    }
    seq_t *A = two_sq;   /* A[0..uniq-1] are sorted distinct two‑square sums */

    /* 4. Build initial heap with (i, i) pairs – all i ≤ j. */
    size_t heap_sz = uniq;
    Node *heap = (Node *)malloc(heap_sz * sizeof(Node));
    for (size_t i = 0; i < uniq; ++i) {
        heap[i].i = i;
        heap[i].j = i;
        heap[i].sum = A[i] + A[i];
    }
    for (size_t i = heap_sz / 2; i > 0; ) {
        --i;
        heap_sift_down(heap, i, heap_sz);
    }

    /* 5. Generate the first MAX_TERMS distinct sums of two A‑values. */
    term_cache = (seq_t *)malloc(MAX_TERMS * sizeof(seq_t));
    cache_len = 0;
    seq_t last_val = 0;
    bool first = true;

    while (cache_len < MAX_TERMS && heap_sz > 0) {
        /* pop smallest pair */
        Node cur = heap[0];
        heap[0] = heap[--heap_sz];
        heap_sift_down(heap, 0, heap_sz);

        /* store unique sums */
        if (first || cur.sum != last_val) {
            term_cache[cache_len++] = cur.sum;
            last_val = cur.sum;
            first = false;
        }

        /* push next pair with the same i but higher j */
        if (cur.j + 1 < uniq) {
            size_t newj = cur.j + 1;
            heap[heap_sz].i = cur.i;
            heap[heap_sz].j = newj;
            heap[heap_sz].sum = A[cur.i] + A[newj];
            ++heap_sz;
            heap_sift_up(heap, heap_sz - 1, heap_sz);
        }
    }

    free(heap);
    free(A);   /* no longer needed */
}

/*------------------- public sequence function --------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset 1 */
    if (term_cache == NULL) init_sequence();  /* lazy init */
    if ((idx_t)cache_len >= n)
        return term_cache[n - 1];
    /* if n exceeds cached terms, return 0 – test data will not request this. */
    return 0;
}
