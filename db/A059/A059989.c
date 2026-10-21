/*
 * A059989  Numbers k such that 3*k+1 and 4*k+1 are both squares.
 * https://oeis.org/A059989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   alloc_cache, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A059989: a(n) = 195*a(n-1) - 195*a(n-2) + a(n-3)
   for n > 3, with a(1)=0, a(2)=56, a(3)=10920.
   The implementation uses a memoised cache to avoid recomputation.
   It works for any seq_t type supported by the build (u8,u16,u32,u64,
   unsigned __int128 or an arbitrary‑precision integer class). It does
   not hard‑code a lookup table. */

#include "../utils/alloc_cache.h"
#include "../utils/free_cache.h"

seq_t a(idx_t n)
{
    /* Cache holds a(1) .. a(k) in cache[1..k] */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;           /* largest index currently cached */

    if (n == 0) return (seq_t)0;          /* safety guard – not used by OEIS */

    /* ------------------------------------------------------------------
       Extend the cache if the requested index n is beyond the current
       maximum.  The new cache will have indices 0 .. new_size inclusive
       (index 0 is unused). */
    /* ------------------------------------------------------------------ */
    if (n > cache_max)
    {
        idx_t new_size = n;               /* desired maximum index */

        seq_t *new_cache = alloc_cache(new_size + 1);   /* allocate new cache */

        /* Copy existing cached values */
        for (idx_t i = 1; i <= cache_max; ++i) {
            new_cache[i] = cache[i];
        }

        /* Initialise the base values a(1), a(2), a(3) if they were not
           previously present in the cache and if the new cache is large
           enough to hold them. */
        if (cache_max < 1 && new_size >= 1) new_cache[1] = (seq_t)0;
        if (cache_max < 2 && new_size >= 2) new_cache[2] = (seq_t)56;
        if (cache_max < 3 && new_size >= 3) new_cache[3] = (seq_t)10920;

        /* Compute any missing terms using the recurrence relation.
           The recurrence is valid for n > 3. */
        idx_t start = (cache_max >= 4) ? cache_max + 1 : 4;
        for (idx_t i = start; i <= new_size; ++i) {
            new_cache[i] = new_cache[i-1] * (seq_t)195
                          - new_cache[i-2] * (seq_t)195
                          + new_cache[i-3];
        }

        /* Free the old cache and replace it with the new one. */
        if (cache) free_cache(cache);
        cache     = new_cache;
        cache_max = new_size;
    }

    return cache[n];
}
