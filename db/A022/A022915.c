/*
 * A022915  Multinomial coefficients (0, 1, ..., n)! =
 *          C(n+1,2)!/(0!*1!*2!*...*n!).
 * https://oeis.org/A022915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) * C(n*(n+1)/2 , n), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* compute C(N,k) for 0 <= k <= N using a multiplicative formula.
 * N is given in idx_t (uint64_t).  Return type seq_t. */
static seq_t binom_simple(idx_t N, idx_t k)
{
    if (k > N - k)            /* symmetry */
        k = N - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply before divide – division is exact */
        res = res * ((seq_t)(N - k + i)) / (seq_t)i;
    }
    return res;
}

/* main sequence function */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoisation table */
    static size_t cache_cap = 0;  /* number of entries allocated */

    /* enlarge cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = n + 1;
        if (new_cap < 16)         /* small initial growth */
            new_cap = 16;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache)                /* allocation failure */
            return 0;
        /* initialise new entries to 0 (works for all accepted seq_t types) */
        for (size_t i = cache_cap; i < new_cap; ++i)
            cache[i] = 0;
        cache_cap = new_cap;
    }

    /* return stored value, or compute it */
    if (cache[n] != 0 || n == 0) {          /* a(0) = 1; other cached values are non‑zero */
        if (n == 0)
            cache[0] = 1;
        return cache[n];
    }

    /* compute a(n-1) recursively (memoised) */
    seq_t prev = a(n - 1);

    /* compute binomial coefficient C( n*(n+1)/2 , n ) */
    seq_t S = (seq_t)n * ((seq_t)n + 1) / 2;
    seq_t bin = binom_simple((idx_t)S, n);

    /* new value */
    seq_t val = prev * bin;
    cache[n] = val;
    return val;
}
