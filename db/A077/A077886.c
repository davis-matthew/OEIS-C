/*
 * A077886  Expansion of (1-x)^(-1)/(1+2*x^3).
 * https://oeis.org/A077886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - 2*a(n-3) + 2*a(n-4), a(0) = 1, a(1) = 1, a(2) =
 *            1, a(3) = -1
 * Range:     i8 n <= 23, i16 n <= 47, i32 n <= 95, i64 n <= 191,
 *            i128 n <= 383, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;               /* pointer to cached values   */
    static idx_t cache_size = 0;              /* next free index in cache   */

    /* Expand cache if needed */
    if (cache_size <= n) {
        /* Determine new size: at least n+1, but keep it a power of two for few reallocs */
        idx_t new_size = (n + 1 > 8) ? (n + 1) : 8;
        if (new_size < cache_size * 2) new_size = cache_size * 2;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (cache == NULL) abort();           /* out of memory */

        /* Fill cache from old_size up to new_size-1 */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i == 0)          cache[i] =  1;
            else if (i == 1)     cache[i] =  1;
            else if (i == 2)     cache[i] =  1;
            else if (i == 3)     cache[i] = -1;
            else
                cache[i] = cache[i-1] - 2*cache[i-3] + 2*cache[i-4];
        }
        cache_size = new_size;
    }

    return cache[n];
}
