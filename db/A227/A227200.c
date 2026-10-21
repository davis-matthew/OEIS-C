/*
 * A227200  a(n) = a(n-1) + a(n-2) - 2^(n-1) with a(0)=a(2)=0, a(1)=-a(3)=1,
 *          a(4)=-5.
 * https://oeis.org/A227200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 2*a(n-3), with a(0)=0, a(1)=1, a(2)=0
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Lazy static memoization of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* capacity of allocated array */
    static size_t cache_size = 0;  /* number of terms actually computed (cache_size = last_index+1) */

    /* Initialise the cache with the first three terms */
    if (cache_cap == 0) {
        cache_cap  = 3;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();       /* handle allocation failure */
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 0;
        cache_size = 3;
    }

    /* Grow the array if the requested index is outside the current capacity */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap *= 2;   /* double until sufficient */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Compute any missing terms up to and including n */
    if ((size_t)n >= cache_size) {
        size_t start = cache_size;
        /* start is at least 3 because we already have a(0), a(1), a(2) */
        for (size_t i = start; i <= (size_t)n; ++i) {
            cache[i] = 3*cache[i-1] - cache[i-2] - 2*cache[i-3];
        }
        cache_size = (size_t)n + 1;
    }

    return cache[n];
}
