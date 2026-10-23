/*
 * A015588  Expansion of x/(1 - 10*x - 3*x^2).
 * https://oeis.org/A015588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + 3*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation across calls */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache if it's the first call */
    if (cache == NULL) {
        cache_size = 2;                      /* at least a(0) and a(1) */
        cache = (seq_t *)realloc(NULL, cache_size * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    /* expand cache if needed */
    if (n >= cache_size) {
        /* new size is n+1 (safety for overflow is not needed for the intended ranges) */
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (new_cache == NULL) { /* allocation failed – abort */
            return (seq_t)0;
        }
        cache = new_cache;
        /* compute missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = (seq_t)10 * cache[i - 1] + (seq_t)3 * cache[i - 2];
        }
        cache_size = n + 1;
    }

    return cache[n];
}
