/*
 * A037582  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 1,3.
 * https://oeis.org/A037582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=13, a(3)=131; a(n)=10*a(n-1)+a(n-2)-10*a(n-3) */

#include <stddef.h>  /* size_t */
#include <stdlib.h>  /* realloc, free */

seq_t a(idx_t n)
{
    /* Handle out‑of‑range requests gracefully */
    if (n == 0)
        return (seq_t)0;

    /* Static cache for memoisation */
    static seq_t  *cache   = NULL; /* Cache array, 1‑based */
    static idx_t   max_nc  = 0;    /* Highest n cached so far */
    static size_t  cap     = 0;    /* Allocated capacity of cache */

    /* ---- Expand the cache if needed ---- */
    if (n > max_nc)
    {
        /* Ensure cache has room for indices up to n */
        size_t needed = (size_t)n + 1; /* index 0 unused */
        if (needed > cap)
        {
            size_t newcap = needed;
            if (newcap < 2 * cap) newcap = 2 * cap; /* growth factor 2 */
            if (newcap < needed)       newcap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) { free(cache); cache = NULL; cap = 0; max_nc = 0; return (seq_t)0; }
            cache = tmp;
            cap   = newcap;
        }

        /* Initialise new cache entries from current max+1 up to n */
        for (idx_t i = max_nc + 1; i <= n; ++i)
        {
            if (i == 1)
                cache[i] = (seq_t)1;
            else if (i == 2)
                cache[i] = (seq_t)13;
            else if (i == 3)
                cache[i] = (seq_t)131;
            else
            {
                seq_t term = (seq_t)10 * cache[i - 1] + cache[i - 2] - (seq_t)10 * cache[i - 3];
                cache[i] = term;
            }
        }
        max_nc = n;
    }

    /* Return the requested term */
    return cache[n];
}
