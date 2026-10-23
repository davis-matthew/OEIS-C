/*
 * A193645  Number of arrays of -5..5 integers x(1..n) with every x(i) in a
 *          subsequence of length 1 or 2 with sum zero.
 * https://oeis.org/A193645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 8*a(n-2) + a(n-3), with a(1)=1, a(2)=11,
 *            a(3)=31. This recurrence is used together with a static cache to
 *            handle any order of calls
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset. */
    if (n < 1)
        return (seq_t)0;

    /* Static cache: cache[0] is unused, cache[1..cache_filled] are valid. */
    static seq_t *cache   = NULL;
    static idx_t cache_filled = 0;   /* last index with a stored value */
    static idx_t cache_size   = 0;   /* current allocated size (>= cache_filled+1) */

    /* Initialise first three terms on first call. */
    if (cache == NULL) {
        /* Allocate space for indices 0..3 (4 slots). */
        cache_size = 4;   /* 0 unused, 1..3 valid */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;     /* allocation failed – return 0 */

        cache[0] = (seq_t)0;     /* unused */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)11;
        cache[3] = (seq_t)31;
        cache_filled = 3;
    }

    /* Extend cache if the requested index is beyond the current stored range. */
    if (n > cache_filled) {
        /* Grow to accommodate index n. */
        idx_t new_size = n + 1;      /* indices 0..n */
        if (new_size > cache_size) {
            seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (!tmp) /* allocation failed – keep previous cache */
                return cache[cache_filled];
            cache = tmp;
            cache_size = new_size;
        }

        /* Compute missing terms using the recurrence. */
        for (idx_t i = cache_filled + 1; i <= n; ++i) {
            cache[i] = 2 * cache[i - 1] + 8 * cache[i - 2] + cache[i - 3];
        }
        cache_filled = n;
    }

    return cache[n];
}
