/*
 * A151919  a(n) = (-2)^n*A_{n,3}(1/2) where A_{n,k}(x) are the generalized
 *          Eulerian polynomials.
 * https://oeis.org/A151919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 13, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = (-1)^n + Σ_{k=1..n} binom(n,k) * (-3)^k * a_{n-k}  */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] = a(i)  */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t  last_seen  = -1;     /* largest n for which cache is filled */

    /* expand cache if needed */
    size_t needed = (size_t)(n + 1);
    if (cache == NULL) {
        cache_cap = needed;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation error */
        for (size_t i = 0; i < cache_cap; ++i) cache[i] = (seq_t)0;
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        last_seen = 0;
    } else if (needed > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (tmp == NULL) return 0;
        cache = tmp;
        for (size_t i = cache_cap; i < needed; ++i) cache[i] = (seq_t)0;
        cache_cap = needed;
    }

    /* compute missing values up to n */
    for (idx_t m = last_seen + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;

        /* we will iterate over k = 1 .. m */
        seq_t binom = (seq_t)m;          /* binom(m,1) = m */
        seq_t pow3  = (seq_t)-3;          /* (-3)^1 */

        for (idx_t k = 1; k <= m; ++k) {
            if (k > 1) {
                /* update binom to binom(m,k) = binom * (m-k+1) / k */
                binom = (binom * (seq_t)(m - k + 1)) / (seq_t)k;
            }

            /* term = binom(m,k) * (-3)^k * a_{m-k} */
            seq_t term = binom * pow3 * cache[(size_t)(m - k)];
            sum += term;

            /* update power of -3 for next k */
            pow3 = pow3 * (seq_t)-3;
        }

        /* add the (-1)^m part */
        sum += (m & 1) ? -(seq_t)1 : (seq_t)1;

        cache[(size_t)m] = sum;
    }

    last_seen = n;
    return cache[(size_t)n];
}
