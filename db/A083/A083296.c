/*
 * A083296  a(n) = (4*3^n + (-7)^n)/5.
 * https://oeis.org/A083296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (4*3^n + (-7)^n)/5
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 11, i64 n <= 23, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_size  = 0;
    static idx_t cache_cap   = 0;

    /* Initialize cache on first call. */
    if (cache == NULL) {
        cache_cap   = 8;
        cache       = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
        cache_size  = 2;
    }

    /* Return cached value if available. */
    if (n < cache_size)
        return cache[n];

    /* Grow cache and compute up to n. */
    while (cache_size <= n) {
        if (cache_size == cache_cap) {
            cache_cap *= 2;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }

        seq_t prev1 = cache[cache_size - 1];
        seq_t prev2 = cache[cache_size - 2];
        cache[cache_size] = prev1 * (-4) + prev2 * 21;
        cache_size++;
    }

    return cache[n];
}
