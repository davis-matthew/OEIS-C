/*
 * A167643  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1. For 1 ≤ n ≤ 13: a(n) = 2 + 44 * Σ_{k=0}^{n-1} a(k).
 *            For n = 14: a(14) = 1 + 44 * Σ_{k=1}^{13} a(k) – 990 * a(0). For
 *            n ≥ 15: a(n) = 44 * Σ_{k=1}^{13} a(n-k) – 990 * a(n-14)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache      = NULL;  /* computed terms */
static idx_t cache_len   = 0;      /* number of computed terms (next free index) */
static idx_t cache_cap   = 0;      /* current capacity of the cache */

/* Ensure the cache can hold indices up to `needed` */
static void ensure_capacity(idx_t needed)
{
    if (needed + 1 <= cache_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed + 1)
        new_cap <<= 1;

    if (cache == NULL) {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    }
    cache_cap = new_cap;
}

/* Compute all terms up to index `n` (inclusive) */
static void compute_to(idx_t n)
{
    if (cache_len > n)
        return;

    /* Grow cache if necessary */
    ensure_capacity(n);

    /* Compute running totals from existing cache */
    seq_t sum_prev = 0;
    for (idx_t i = 0; i < cache_len; ++i)
        sum_prev += cache[i];

    seq_t sum13 = 0;
    if (cache_len > 1) {
        idx_t start = (cache_len > 13) ? (cache_len - 13) : 1;
        for (idx_t i = start; i < cache_len; ++i)
            sum13 += cache[i];
    }

    /* Generate new terms */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t val;

        if (k == 0) {
            val = (seq_t)1;
        } else if (k <= 13) {
            val = (seq_t)2 + (seq_t)44 * sum_prev;
        } else if (k == 14) {
            val = (seq_t)1 + (seq_t)44 * sum13 - (seq_t)990 * cache[0];
        } else {                 /* k > 14 */
            val = (seq_t)44 * sum13 - (seq_t)990 * cache[k - 14];
        }

        cache[k] = val;

        /* Update running sums for the next iteration */
        if (k == 0) {
            sum_prev = val;
        } else if (k <= 13) {
            sum_prev += val;
            sum13   += val;
        } else {                     /* k ≥ 14 */
            sum13   += val;           /* add the new term */
            sum13   -= cache[k - 13]; /* drop the term leaving the window */
        }
    }

    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    compute_to(n);
    return cache[n];
}
