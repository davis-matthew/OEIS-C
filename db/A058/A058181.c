/*
 * A058181  Quadratic recurrence a(n) = a(n-1)^2 - a(n-2) for n >= 2 with a(0)
 *          = 1 and a(1) = 0.
 * https://oeis.org/A058181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1, a(1) = 0, a(n) = a(n-1)^2 - a(n-2) for n ≥ 2
 * Range:     i8 n <= 7, i16 n <= 8, i32 n <= 9, i64 n <= 10, i128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc / realloc */

seq_t a(idx_t n)
{
    /* static cache of previously computed values, indexed from 0 */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* number of elements currently stored */

    /* handle the two base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* initialise cache with the first two terms if not yet done */
    if (cache_size <= 1) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        /* assume malloc succeeds; a real implementation should check the result */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache_size = 2;
    }

    /* grow the cache and compute any missing terms up to n */
    if (n >= cache_size) {
        idx_t old_size = cache_size;
        idx_t new_size = n + 1;   /* inclusive of n */
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        /* compute terms in order, reusing previous two values */
        for (idx_t i = old_size; i < new_size; ++i) {
            seq_t prev1 = cache[i - 1];
            seq_t prev2 = cache[i - 2];
            cache[i] = prev1 * prev1 - prev2;
        }
        cache_size = new_size;
    }

    /* return the requested term */
    return cache[n];
}
