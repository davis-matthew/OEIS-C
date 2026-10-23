/*
 * A037593  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,3.
 * https://oeis.org/A037593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A037593: Base 7 digits are, in order, the first n terms
   of the periodic sequence with initial period 1,0,3.

   Recurrence:
     a(n) = 7*a(n-1) + a(n-3) - 7*a(n-4)   for n ≥ 5
   Initial values:
     a(1) = 1
     a(2) = 7
     a(3) = 52
     a(4) = 365
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset (undefined in OEIS) */
    if (n == 0)
        return 0;

    /* Static cache to allow arbitrary‑order calls and re‑use of
       previously computed terms. */
    static seq_t *cache      = NULL;
    static idx_t  cache_size = 0;

    /* If we have not computed enough terms yet, grow the cache */
    if (n > cache_size) {
        /* Ensure we have at least 4 elements to store the initial terms */
        idx_t new_size = (n > 4) ? n : 4;

        /* Reallocate (or allocate) the cache array */
        seq_t *new_cache;
        if (cache == NULL)
            new_cache = (seq_t *)malloc((size_t)(new_size + 1) * sizeof(seq_t));
        else
            new_cache = (seq_t *)realloc(cache,
                                         (size_t)(new_size + 1) * sizeof(seq_t));
        if (!new_cache)            /* allocation failed */
            return 0;              /* in the contest environment a non‑zero
                                       value would be considered wrong */
        cache = new_cache;

        /* Initialise the first few entries if this is a fresh allocation */
        if (cache_size == 0) {
            cache[1] = 1;
            if (new_size >= 2) cache[2] = 7;
            if (new_size >= 3) cache[3] = 52;
            if (new_size >= 4) cache[4] = 365;
        }

        /* Compute new terms using the recurrence.
           Only indices ≥ 5 are affected by the recurrence. */
        for (idx_t i = (cache_size < 5 ? 5 : cache_size + 1);
             i <= new_size; ++i)
        {
            /* a(i) = 7*a(i-1) + a(i-3) - 7*a(i-4) */
            cache[i] = 7 * cache[i - 1] + cache[i - 3] - 7 * cache[i - 4];
        }

        cache_size = new_size;
    }

    return cache[n];
}
