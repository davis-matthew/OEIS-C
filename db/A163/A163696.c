/*
 * A163696  Number of n X 3 binary arrays with all 1s connected, a path of 1s
 *          from top row to lower right corner, and no 1 having more than two
 *          1s adjacent.
 * https://oeis.org/A163696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + 2*a(n-3) + a(n-4) - 2*a(n-5) +
 *            a(n-6) for n >= 10 initial terms:
 *            a(1)=3,a(2)=11,a(3)=28,a(4)=63,a(5)=138,a(6)=298,a(7)=642,a(8)=1371,a(9)=2908
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 28, u64 n <= 57, u128 n <= 117,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* Helper array holding the first nine terms */
    static const seq_t init[10] = {
        0,   /* unused 0‑th element */
        3,   /* a(1) */
        11,  /* a(2) */
        28,  /* a(3) */
        63,  /* a(4) */
        138, /* a(5) */
        298, /* a(6) */
        642, /* a(7) */
        1371,/* a(8) */
        2908 /* a(9) */
    };

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last = 0;          /* highest index stored */

    /* trivial case: the first nine elements are known */
    if (n <= 9)
        return init[(size_t)n];

    /* initialise cache upon first use */
    if (cache == NULL)
    {
        /* be sure we can address indices up to 9 */
        ensure_cache((idx_t)9, &cache, &cap);
        for (size_t i = 1; i <= 9; ++i)
            cache[i] = init[i];
        last = 9;
    }

    /* extend the cache if a higher index is requested */
    if (n > last)
    {
        /* guarantee enough space */
        ensure_cache(n, &cache, &cap);

        /* compute new terms using the recurrence */
        for (idx_t k = last + 1; k <= n; ++k)
        {
            cache[k] =   4 * cache[k - 1]
                       -5 * cache[k - 2]
                       +2 * cache[k - 3]
                       +  cache[k - 4]
                       -2 * cache[k - 5]
                       +  cache[k - 6];
        }
        last = n;
    }

    return cache[n];
}
