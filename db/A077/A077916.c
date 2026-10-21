/*
 * A077916  Expansion of (1-x)^(-1)/(1 + 2*x - 2*x^2 - x^3).
 * https://oeis.org/A077916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + 4*a(n-2) - a(n-3) - a(n-4), with a(0)=1,
 *            a(1)=-1, a(2)=5, a(3)=-10
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 22, i64 n <= 45, i128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>


seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_alloc = 0;
    static size_t cache_count = 0;

    size_t idx = (size_t)n;

    /* initialise base terms on first call */
    if (cache_alloc == 0) {
        cache_alloc = 4;
        cache = (seq_t *)malloc(cache_alloc * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = -1;
        cache[2] = 5;
        cache[3] = -10;
        cache_count = 4;
    }

    /* if requested term already computed, return it */
    if (idx < cache_count)
        return cache[idx];

    /* grow the cache if necessary */
    if (idx >= cache_alloc) {
        size_t new_alloc = cache_alloc;
        while (idx >= new_alloc)
            new_alloc <<= 1;          /* double until enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        cache = tmp;
        cache_alloc = new_alloc;
    }

    /* compute any missing terms up to idx */
    for (size_t i = cache_count; i <= idx; ++i) {
        cache[i] = -cache[i-1] + 4*cache[i-2] - cache[i-3] - cache[i-4];
    }
    cache_count = idx + 1;
    return cache[idx];
}
