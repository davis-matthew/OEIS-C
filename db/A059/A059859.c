/*
 * A059859  Sum of squares of first n quarter-squares (A002620).
 * https://oeis.org/A059859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} floor(i^2/4)^2 (sum of squares of
 *            quarter-squares)
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 202, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values, cache[0] = 0  */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache if first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) abort();          /* simple OOM handling */
        cache[0] = 0;                        /* a(0) = 0 */
        cache_size = 1;
    }

    /* extend cache up to n if necessary */
    if (n >= cache_size) {
        idx_t i;
        idx_t new_size = n + 1;              /* need indices 0..n */
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (new_cache == NULL) abort();
        cache = new_cache;

        /* compute missing terms */
        for (i = cache_size; i <= n; ++i) {
            /* floor(i^2/4) = floor(i/2) * ceil(i/2) */
            seq_t a = (seq_t)(i >> 1);          /* floor(i/2)   */
            seq_t b = (seq_t)((i + 1) >> 1);   /* ceil(i/2)    */
            seq_t prod = a * b;                /* quarter-square value */
            cache[i] = cache[i - 1] + prod * prod; /* cumulative sum */
        }
        cache_size = new_size;
    }

    return cache[n];
}
