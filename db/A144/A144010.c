/*
 * A144010  E.g.f. satisfies: A'(x) = 1/(1 - x*A(x)) with A(0)=1.
 * https://oeis.org/A144010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{0 < k < n} k * binomial(n-1,k) * a(k) * a(n-k-1),
 *            with a(0) = a(1) = 1. This implements a memoised O(n²)
 *            recurrence
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static idx_t  cache_cap = 0;   /* size of allocated array          */
    static idx_t  cache_used = 0;  /* highest index that has been computed */

    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)1;

    /* initialise or enlarge cache */
    if (cache == NULL) {
        cache_cap  = n + 1;            /* at least n+1 elements  */
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0]   = (seq_t)1;
        cache[1]   = (seq_t)1;
        cache_used = 1;
    } else if (n >= cache_cap) {
        /* increase capacity to at least n+1, doubling if possible */
        cache_cap = n + 1;
        cache     = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* compute all required values up to n */
    for (idx_t m = cache_used + 1; m <= n; ++m) {
        seq_t sum  = 0;
        idx_t N    = m - 1;          /* we need binomials of N */
        seq_t binom = 1;             /* will hold C(N, k) */

        for (idx_t k = 1; k < m; ++k) {
            /* update binomial to C(N, k) */
            binom = binom * ((seq_t)(N - (k - 1))) / ((seq_t)k);

            /* term = k * binom * a(k) * a(m-k-1) */
            seq_t term = (seq_t)k * binom;
            term = term * cache[k];
            term = term * cache[m - k - 1];

            sum += term;
        }
        cache[m] = sum;
    }

    cache_used = n;
    return cache[n];
}
