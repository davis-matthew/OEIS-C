/*
 * A077240  Bisection (even part) of Chebyshev sequence with Diophantine
 *          property.
 * https://oeis.org/A077240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=5, a(1)=23, a(n)=6*a(n-1)-a(n-2) for n>=2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* ensure the cache contains terms up to index n */
    if (n >= cache_len) {
        if (cache_len == 0) {
            /* initialise cache with first two terms */
            cache_cap = 2;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            cache[0] = 5;
            cache[1] = 23;
            cache_len = 2;
        }

        /* extend cache up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* grow capacity if needed */
            if (i >= cache_cap) {
                cache_cap = (cache_cap == 0) ? 2 : cache_cap * 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }

            if (i == 0) {
                cache[i] = 5;
            } else if (i == 1) {
                cache[i] = 23;
            } else {
                seq_t t1 = cache[i - 1] * (seq_t)6;
                cache[i] = t1 - cache[i - 2];
            }
            cache_len = i + 1;
        }
    }

    return cache[n];
}
