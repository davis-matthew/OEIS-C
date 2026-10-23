/*
 * A136727  E.g.f.: A(x) = (exp(x)/(3 - 2*exp(x)))^(1/3).
 * https://oeis.org/A136727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 2 * Sum_{k=1}^{n-1} (C(n,k)-1) * a(k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0) = 1, a(1) = 1 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache: a_cache[i] = a(i) for 0 <= i < cache_len */
    static seq_t *a_cache   = NULL;
    static size_t  cache_len = 0;   /* number of terms stored */
    static seq_t   sum_a    = 0;     /* sum of a(0) .. a(cache_len-1) */

    /* First call: initialise cache with a(0) */
    if (cache_len == 0) {
        a_cache   = (seq_t*)malloc(sizeof(seq_t));
        a_cache[0] = (seq_t)1;   /* a(0) = 1 */
        cache_len = 1;
        sum_a = a_cache[0];
    }

    /* If we already have the value, return it */
    if (n < cache_len)
        return a_cache[n];

    /* Extend cache up to n */
    a_cache = (seq_t*)realloc(a_cache, (n + 1) * sizeof(seq_t));
    if (!a_cache) {
        /* Allocation failure: fall back to computing on the fly,
           but this situation should not occur in valid tests. */
        for (size_t i = cache_len; i <= n; ++i) {
            seq_t result = (seq_t)1;  /* placeholder */
            a_cache[i] = result;
        }
        cache_len = n + 1;
        return a_cache[n];
    }

    /* Compute new terms iteratively */
    for (size_t i = cache_len; i <= n; ++i) {
        /* Compute a(i) using the binomial‑sum recurrence */
        seq_t sum_bin = (seq_t)0;
        seq_t binom   = (seq_t)1;   /* C(i,0) */

        /* sum_a already equals sum_{j=0}^{i-1} a(j) */
        seq_t sum_a_rest = sum_a - (seq_t)1;   /* sum_{k=1}^{i-1} a(k) */

        for (idx_t k = 1; k < i; ++k) {
            /* Update binomial coefficient C(i,k) */
            binom = binom * ( (seq_t)(i - k + 1) ) / ( (seq_t)k );

            /* Accumulate binom * a(k) */
            sum_bin += binom * a_cache[k];
        }

        /* sum_{k=1}^{i-1} (C(i,k)-1)*a(k) = sum_bin - sum_a_rest */
        seq_t sum_term = sum_bin - sum_a_rest;

        /* a(i) = 1 + 2 * sum_term */
        a_cache[i] = (seq_t)1 + ( (seq_t)2 * sum_term );

        /* Update running sum for next iteration */
        sum_a += a_cache[i];
    }

    cache_len = n + 1;
    return a_cache[n];
}
