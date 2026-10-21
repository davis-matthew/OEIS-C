/*
 * A234833  Number of tilings of a box with sides 2 X 2 X 3n in R^3 by boxes
 *          of sides Tricube-V(3-dimensional dominoes).
 * https://oeis.org/A234833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 68*a(n-1) - 192*a(n-2), a(0) = 1, a(1) = 44
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;   /* number of valid entries in cache */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* ensure the cache can hold n+1 entries */
    if (n >= cache_len) {
        size_t needed = (size_t)(n + 1);
        if (needed > cache_cap) {
            seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
            if (!new_cache) abort();
            cache = new_cache;
            cache_cap = needed;
        }

        /* compute new entries */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)44;
            } else {
                cache[i] = cache[i-1] * (seq_t)68 - cache[i-2] * (seq_t)192;
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
