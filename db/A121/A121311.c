/*
 * A121311  a(0) = 1, a(1) = 2, a(2) = 5; for n >= 3, a(n) = a(n - 1) - 2*a(n
 *          - 2) + a(n - 3).
 * https://oeis.org/A121311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - 2*a(n-2) + a(n-3); Initial values: a(0)=1,
 *            a(1)=2, a(2)=5
 * Range:     i8 n <= 15, i16 n <= 35, i32 n <= 48, i64 n <= 48, i128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;       /* number of elements currently cached */

    /* Ensure the cache is large enough for index n */
    if (n >= cache_size) {
        /* The required size must be at least 3 to hold the base terms */
        idx_t new_size = n + 1;
        if (new_size < 3)
            new_size = 3;

        /* Allocate or grow the cache */
        seq_t *tmp;
        if (!cache) {
            tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
            if (!tmp) abort();
        } else {
            tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (!tmp) abort();
        }
        cache = tmp;

        /* Initialise base values on first allocation */
        if (cache_size == 0) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)2;
            cache[2] = (seq_t)5;
        }

        /* Compute any new entries using the recurrence */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i < 3) continue;  /* Already initialized */
            cache[i] = cache[i - 1] - (seq_t)2 * cache[i - 2] + cache[i - 3];
        }

        cache_size = new_size;
    }

    return cache[(size_t)n];
}
