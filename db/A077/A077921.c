/*
 * A077921  Expansion of (1-x)^(-1)/(1+2*x-x^2).
 * https://oeis.org/A077921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1, a(1) = -1, a(2) = 4 a(n) = -a(n-1) + 3*a(n-2) - a(n-3)
 *            for n > 2
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 24, i64 n <= 50, i128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values
       allows calls in arbitrary order and O(n) total work */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;       /* number of values stored */
    static idx_t cache_capacity = 0;   /* allocated capacity */

    /* initialise cache with first three terms */
    if (cache == NULL) {
        cache_capacity = 3;
        cache_size = 3;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)4;
    }

    /* return value if already cached */
    if (n < (idx_t)cache_size) {
        return cache[(size_t)n];
    }

    /* extend cache up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* ensure enough space */
        if (cache_size >= cache_capacity) {
            cache_capacity = (cache_capacity > 0) ? cache_capacity * 2 : 4;
            cache = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
        }
        /* recurrence: a(i) = -a(i-1) + 3*a(i-2) - a(i-3) */
        seq_t val = -cache[i - 1];
        val += (seq_t)3 * cache[i - 2];
        val -= cache[i - 3];
        cache[cache_size++] = val;
    }
    return cache[(size_t)n];
}
