/*
 * A077954  Expansion of 1/(1-x+2*x^2-x^3) in powers of x.
 * https://oeis.org/A077954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1, a(1) = 1, a(2) = -1, a(n) = a(n-1) - 2*a(n-2) + a(n-3)
 *            for n ≥ 3
 * Range:     i8 n <= 17, i16 n <= 37, i32 n <= 77, i64 n <= 156,
 *            i128 n <= 314, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_capacity = 0;
    static idx_t cache_last = 0;   /* largest index computed so far */

    /* initialise cache with the first three terms */
    if (cache == NULL) {
        cache_capacity = 3;
        cache = (seq_t*)malloc((size_t)cache_capacity * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)-1;
        cache_last = 2;
    }

    /* grow array if necessary */
    if ((idx_t)(cache_capacity - 1) < n) {
        idx_t new_capacity = cache_capacity * 2;
        if ((idx_t)(new_capacity - 1) < n)
            new_capacity = n + 1;
        cache = (seq_t*)realloc(cache, (size_t)new_capacity * sizeof(seq_t));
        cache_capacity = new_capacity;
    }

    /* compute missing values up to n */
    if (cache_last < n) {
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            cache[i] = cache[i-1] - (seq_t)2 * cache[i-2] + cache[i-3];
        }
        cache_last = n;
    }

    return cache[n];
}
