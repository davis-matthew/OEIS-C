/*
 * A028145  Expansion of 1/((1-4x)(1-7x)(1-8x)(1-9x)).
 * https://oeis.org/A028145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 28*a(n-1) - 287*a(n-2) + 1268*a(n-3) - 2016*a(n-4) with
 *            a(0)=1, a(1)=28, a(2)=497, a(3)=7148
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;     /* array of terms */
    static size_t cap   = 0;        /* allocated capacity (number of elements) */
    static size_t len   = 0;        /* number of valid terms stored */

    size_t target = (size_t)n + 1;           /* how many terms we need (a[0..n]) */

    /* Expand the allocation if necessary.  seq_realloc handles the
       construction and destruction of seq_t objects safely. */
    if (target > cap)
    {
        cache = seq_realloc(cache, cap, target);
        cap   = target;
    }

    if (len < target)                    /* we still have missing terms */
    {
        /* initialise missing base terms, taking care not to write past
           the current capacity.  The maximum needed in this block is 4. */
        size_t up_to = target < 4 ? target : 4;

        if (len == 0)
        {
            cache[0] = 1;
            if (up_to > 1) cache[1] = 28;
            if (up_to > 2) cache[2] = 497;
            if (up_to > 3) cache[3] = 7148;
            len = up_to;
        }
        else if (len < 4)
        {
            if (len <= 1 && up_to > 1) cache[1] = 28;
            if (len <= 2 && up_to > 2) cache[2] = 497;
            if (len <= 3 && up_to > 3) cache[3] = 7148;
            len = up_to;
        }

        /* Now compute all remaining terms up to n using the recurrence. */
        for (size_t i = len; i < target; ++i)
        {
            cache[i] = 28 * cache[i-1]
                       - 287 * cache[i-2]
                       + 1268 * cache[i-3]
                       - 2016 * cache[i-4];
        }
        len = target;                    /* all required terms are stored */
    }

    return cache[(size_t)n];
}
