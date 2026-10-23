/*
 * A170366  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies a linear recurrence derived from the * rational
 *            generating function of the Coxeter group. * * The recurrence is:
 *            * a(0) = 1 * for 1 ≤ n ≤ 42: a(n) = 2 + 19 * Σ_{i=0}^{n-1} a(i)
 *            * a(43) = 19 * Σ_{i=0}^{42} a(i) - 208 * for n ≥ 44: a(n) = 19 *
 *            Σ_{i=n-42}^{n-1} a(i) - 190 * a(n-43) *...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache   = NULL; /* array of a(i) values */
static seq_t *cum_cache = NULL; /* cumulative sums of a(i) */
static idx_t  cache_len = 0;    /* number of initialized terms */
static size_t cache_cap = 0;    /* allocated capacity */

/* Ensure the cache holds values up to n inclusive. */
static void ensure_cache(idx_t n)
{
    if (cache_len > n) return;          /* already done */

    /* Allocate or grow the cache */
    if (cache_cap == 0) {
        cache_cap = 16;                 /* initial capacity */
        a_cache   = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cum_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    while (cache_cap <= n) {
        cache_cap *= 2;
        a_cache   = (seq_t *)realloc(a_cache,   cache_cap * sizeof(seq_t));
        cum_cache = (seq_t *)realloc(cum_cache, cache_cap * sizeof(seq_t));
    }

    /* Compute terms from cache_len up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            a_cache[0] = (seq_t)1;
            cum_cache[0] = a_cache[0];
        } else if (i <= 42) {
            /* a(i) = 2 + 19 * Σ_{j=0}^{i-1} a(j) = 2 + 19 * cum(i-1) */
            a_cache[i] = (seq_t)2 + (seq_t)19 * cum_cache[i-1];
            cum_cache[i] = cum_cache[i-1] + a_cache[i];
        } else if (i == 43) {
            /* a(43) = 19 * Σ_{j=0}^{42} a(j) - 208 = 19 * cum(42) - 208 */
            a_cache[43] = (seq_t)19 * cum_cache[42] - (seq_t)208;
            cum_cache[43] = cum_cache[42] + a_cache[43];
        } else {
            /* recurrence: a(n) = 19 * Σ_{k=n-42}^{n-1} a(k) - 190 * a(n-43) */
            seq_t sum_last_42 = cum_cache[i-1] - cum_cache[i-43];
            a_cache[i] = (seq_t)19 * sum_last_42 - (seq_t)190 * a_cache[i-43];
            cum_cache[i] = cum_cache[i-1] + a_cache[i];
        }
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
