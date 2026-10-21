/*
 * A205329  Number of (n+1) X 3 0..2 arrays with the number of equal 2 X 2
 *          subblock diagonal pairs and equal antidiagonal pairs differing
 *          from each horizontal or vertical neighbor, and new values 0..2
 *          introduced in row major order.
 * https://oeis.org/A205329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 18*a(n-2), a(1) = 72, a(2) = 594
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses memoisation with a dynamic array.  The array
 * grows as needed and the recurrence is evaluated exactly using the
 * type seq_t.  The code works in both C11 and C++17. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Convert the unsigned 64‑bit index to a size_t for array indexing. */
    size_t idx = (size_t)n;
    if (idx < 1) return (seq_t)0;   /* undefined for n < 1, guard */

    /* Static cache of computed values.  cache_len is the highest index
     * that has been computed (at least 0).  cache_cap is the allocated
     * capacity.  cache[0] is unused. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Ensure the array is large enough to hold a(idx). */
    if (idx + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < idx + 1) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) exit(1);   /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first two known values if the cache is not yet ready. */
    if (cache_len < 2) {
        cache[1] = (seq_t)72;   /* a(1) */
        cache[2] = (seq_t)594;  /* a(2) */
        cache_len = 2;
    }

    /* Compute any missing terms up to a(idx). */
    if (idx > cache_len) {
        for (size_t i = cache_len + 1; i <= idx; ++i) {
            cache[i] = cache[i - 1] * (seq_t)6 + cache[i - 2] * (seq_t)18;
        }
        cache_len = idx;
    }

    return cache[idx];
}
