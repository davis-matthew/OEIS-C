/*
 * A218731  a(n) = (28^n - 1)/27.
 * https://oeis.org/A218731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (28^n - 1)/27 (floor(28^n/27))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0, a(1) = 1, a(n) = 29*a(n-1) - 28*a(n-2) for n ≥ 2 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: array of computed values */
    static seq_t *cache = NULL;    /* pointer to cache array */
    static size_t cache_cap = 0;   /* current capacity of the array */
    static size_t cache_size = 0;  /* number of values currently stored (next free index) */

    /* Ensure we have storage for indices 0 .. n */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < needed) new_cap = needed;
        if (new_cap < 2) new_cap = 2;          /* at least room for a(0) and a(1) */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                    /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise first two terms if cache is empty */
    if (cache_size == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_size = 2;
    }

    /* Compute values up to n if necessary */
    while (cache_size <= (size_t)n) {
        /* a(i) = 29*a(i-1) - 28*a(i-2) */
        seq_t term = cache[cache_size - 1] * 29 - cache[cache_size - 2] * 28;
        cache[cache_size] = term;
        ++cache_size;
    }

    return cache[n];
}
