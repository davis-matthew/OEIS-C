/*
 * A056158  Equivalent of the Kurepa hypothesis for left factorial.
 * https://oeis.org/A056158
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -(n-3)*a(n-1) - 2*(n-1) with a(3) = -4
 * Range:     i8 n <= 8, i16 n <= 11, i32 n <= 15, i64 n <= 23, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements an iterative memoised recurrence that computes terms for any n ≥ 3. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 3) {
        /* Undefined for n < 3; return 0 as a placeholder. */
        return 0;
    }

    /* Static cache: cache[i] holds a(i).  cache_size is the highest
       index that has been computed + 1.  cache_cap is the allocated size. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap = 0;

    /* Initialise the cache once, with indices 0..3.  Indices 0-2 are unused. */
    if (!cache) {
        cache_cap = 8;                /* initial capacity */
        cache = (seq_t *)calloc((size_t)cache_cap, sizeof(seq_t));
        if (!cache) exit(1);
        cache[3] = -(seq_t)4;         /* a(3) = -4 */
        cache_size = 4;               /* indices 0..3 are set */
    }

    /* If requested index is beyond the current capacity, grow the array. */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap * 2;
        while (new_cap <= n) new_cap *= 2;

        seq_t *tmp = (seq_t *)realloc((void *)cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n. */
    if (n >= cache_size) {
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t term = -((seq_t)(i - 3) * prev) - (seq_t)(2 * (i - 1));
            cache[i] = term;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
