/*
 * A056326  Number of reversible string structures with n beads using exactly
 *          two different colors.
 * https://oeis.org/A056326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 6*a(n-3) + 4*a(n-4) * with initial values *
 *            a(1) = 0, a(2) = 1, a(3) = 2, a(4) = 5 * * The function is
 *            memoised on the first call and any subsequent * out‑of‑order
 *            calls. It works for any unsigned integer type seq_t * (8, 16,
 *            32, 64, 128 bits or an arbitrary‑precision class) and...
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* static memoisation cache */
    static seq_t *cache        = NULL;
    static idx_t cache_size    = 0;    /* allocated size (indices 0 … cache_size-1) */
    static idx_t cache_highest = 0;    /* largest index with a valid value */

    if (n == 0)                   /* offset starts at 1, return 0 safely for 0 */
        return 0;

    /* initialise cache on first use */
    if (cache == NULL)
    {
        /* Allocate enough space for the requested index plus the four base
         * values (indices 1…4).  Index 0 is unused. */
        cache_size = (n >= 4 ? n + 1 : 5);
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache)
            abort();

        /* Base values */
        cache[0] = 0;                /* unused */
        cache[1] = 0;
        if (cache_size > 2) cache[2] = 1;
        if (cache_size > 3) cache[3] = 2;
        if (cache_size > 4) cache[4] = 5;

        cache_highest = (n >= 4 ? 4 : n);
    }

    /* grow cache if the requested index is beyond current capacity */
    if (n >= cache_size)
    {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            abort();
        cache       = tmp;
        cache_size  = (idx_t)new_size;
    }

    /* compute missing terms using the recurrence */
    if (n > cache_highest)
    {
        idx_t start = (cache_highest < 5) ? 5 : cache_highest + 1;
        for (idx_t i = start; i <= n; ++i)
        {
            cache[i] = 3 * cache[i - 1] - 6 * cache[i - 3] + 4 * cache[i - 4];
        }
        cache_highest = n;
    }

    return cache[n];
}
