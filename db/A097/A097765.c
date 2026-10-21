/*
 * A097765  Chebyshev U(n,x) polynomial evaluated at x=243=2*11^2+1.
 * https://oeis.org/A097765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 486 * a(n-1) - a(n-2), a(0) = 1, a(1) = 486
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;   /* array a[i] for 0 <= i < cache_len */
    static idx_t cache_len = 0;   /* number of stored values */

    /* ensure cache[0] exists */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* fallback: allocation failed */
        cache[0] = (seq_t)1;
    }

    /* grow cache if needed */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* fallback: allocation failed */
        cache = new_cache;
        /* compute new values */
        for (idx_t i = cache_len; i < new_len; ++i) {
            seq_t prev1 = cache[i - 1];
            seq_t prev2 = (i >= 2) ? cache[i - 2] : (seq_t)0;
            cache[i] = (seq_t)486 * prev1 - prev2;
        }
        cache_len = new_len;
    }

    return cache[n];
}
