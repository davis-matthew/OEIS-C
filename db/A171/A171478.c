/*
 * A171478  a(n) = 6*a(n-1) - 8*a(n-2) + 2 for n > 1; a(0) = 1, a(1) = 8.
 * https://oeis.org/A171478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 8,
   a(n) = 6*a(n-1) - 8*a(n-2) + 2  for n > 1 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Memoisation cache: filled on demand */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap   = 0;

    /* First call: initialise cache with a(0) and a(1) */
    if (cache == NULL)
    {
        cache_cap   = 2;
        cache_size  = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
    }

    /* Extend cache up to requested index if necessary */
    if (n >= cache_size)
    {
        idx_t needed = n + 1;

        /* Grow capacity if needed */
        if (needed > cache_cap)
        {
            idx_t new_cap = cache_cap;
            while (new_cap < needed) new_cap <<= 1;
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* Compute missing terms using the recurrence */
        for (idx_t i = cache_size; i < needed; ++i)
        {
            cache[i] = (seq_t)6 * cache[i-1]
                     - (seq_t)8 * cache[i-2]
                     + (seq_t)2;
        }
        cache_size = needed;
    }

    return cache[n];
}
