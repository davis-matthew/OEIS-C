/*
 * A166927  a(n) = 20*a(n-1) - 64*a(n-2) for n > 1; a(0) = 1, a(1) = 18.
 * https://oeis.org/A166927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - 64*a(n-2), a(0) = 1, a(1) = 18
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 200 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* number of cached entries */

    /* If the cache is not big enough, grow it */
    if ((size_t)(n + 1) > cap)
    {
        size_t newcap = (size_t)(n + 1);
        seq_t *p = mem_resize(seq_t, cache, newcap);
        if (!p) return 0; /* allocation failure – caller should handle it */
        cache = p;

        /* initialise the first terms on a freshly created cache */
        if (cap == 0)
        {
            cache[0] = (seq_t)1;
            if (newcap > 1) cache[1] = (seq_t)18;
        }
        else if (cap == 1)
        {
            if (newcap > 1) cache[1] = (seq_t)18;
        }

        /* compute missing terms using the recurrence */
        for (size_t i = cap; i < newcap; ++i)
        {
            if (i >= 2)
                cache[i] = (seq_t)20 * cache[i - 1] - (seq_t)64 * cache[i - 2];
        }

        cap = newcap;
    }

    return cache[(size_t)n];
}
