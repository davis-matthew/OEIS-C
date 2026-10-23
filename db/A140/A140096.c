/*
 * A140096  a(n) = A000045(n) - A113405(n).
 * https://oeis.org/A140096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n)=+3*a(n-1)-a(n-2)-3*a(n-3)+3*a(n-4)-a(n-5)-2*a(n-6) a(0)=0,
 *            a(1)=1, a(2)=1, a(3)=1, a(4)=1, a(5)=1, a(6)=1
 * Range:     i8 n <= 10, i16 n <= 18, i32 n <= 34, i64 n <= 66, i128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cap = 0;

    /* Initial terms */
    static const seq_t init[7] = { (seq_t)0, (seq_t)1, (seq_t)1,
                                   (seq_t)1, (seq_t)1, (seq_t)1,
                                   (seq_t)1 };

    /* Lazy initialization of the cache */
    if (cache == NULL) {
        /* start with at least one slot beyond the initial values */
        cap = 8;
        cache_size = 7;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (size_t i = 0; i < 7; ++i)
            cache[i] = init[i];
        for (size_t i = 7; i < cap; ++i)
            cache[i] = (seq_t)0;
    }

    /* Grow the cache if the requested index is beyond the current size */
    if ((idx_t)cache_size <= n) {
        /* Determine new capacity (double until it fits) */
        size_t new_cap = cap;
        while ((idx_t)new_cap <= n)
            new_cap = new_cap ? new_cap * 2 : 16;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
        for (size_t i = cache_size; i < cap; ++i)
            cache[i] = (seq_t)0;

        /* Compute new terms using the recurrence */
        for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
            /* recurrence: a(n)=+3*a(n-1)-a(n-2)-3*a(n-3)+3*a(n-4)-a(n-5)-2*a(n-6) */
            seq_t val = 3 * cache[(size_t)i - 1]
                       - cache[(size_t)i - 2]
                       - 3 * cache[(size_t)i - 3]
                       + 3 * cache[(size_t)i - 4]
                       - cache[(size_t)i - 5]
                       - 2 * cache[(size_t)i - 6];
            cache[(size_t)i] = val;
        }
        cache_size = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
