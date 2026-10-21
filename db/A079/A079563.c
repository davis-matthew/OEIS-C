/*
 * A079563  a(n) = a(n,m) = Sum_{k=0..n} binomial(m*k,k)*binomial(m*(n-k),n-k)
 *          for m=7.
 * https://oeis.org/A079563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(7*k, k) * binomial(7*(n-k), n-k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Static cache for binomial(7*k, k) and for the sequence values */
    static seq_t *f = NULL;        /* f[k] = binomial(7*k, k)          */
    static size_t f_len = 0;       /* current allocated length of f      */
    static seq_t *cache = NULL;   /* cache[n] = a(n)                   */
    static size_t cache_len = 0;   /* current allocated length of cache  */

    size_t m = (size_t)n;          /* index requested, fits in size_t */

    /* Initialise the caches on the first call */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;        /* silently fail if out of memory */
        cache[0] = 1;                /* a(0) = 1 */
    }
    if (f_len == 0) {
        f_len = 1;
        f = (seq_t *)malloc(sizeof(seq_t));
        if (!f) return 0;
        f[0] = 1;                    /* binomial(0,0) = 1 */
    }

    /* Grow the cache up to the requested index */
    while (cache_len <= m) {
        /* Make sure binomial(7*k, k) is available up to k = m */
        if (f_len <= m) {
            size_t new_f_len = m + 1;
            seq_t *tmp = (seq_t *)realloc(f, new_f_len * sizeof(seq_t));
            if (!tmp) return 0;
            f = tmp;
            for (size_t k = f_len; k <= m; ++k)
                f[k] = binom((idx_t)(7 * k), (idx_t)k);
            f_len = new_f_len;
        }

        /* Compute a(m) as convolution of f with itself */
        seq_t sum = 0;
        for (size_t k = 0; k <= m; ++k)
            sum += f[k] * f[m - k];

        /* Append the new term to the cache */
        size_t new_cache_len = cache_len + 1;
        seq_t *tmp_cache = (seq_t *)realloc(cache, new_cache_len * sizeof(seq_t));
        if (!tmp_cache) return 0;
        cache = tmp_cache;
        cache[cache_len] = sum;
        cache_len = new_cache_len;
    }

    return cache[m];
}
