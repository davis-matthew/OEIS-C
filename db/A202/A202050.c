/*
 * A202050  Number of (n+2) X 8 binary arrays avoiding patterns 001 and 110 in
 *          rows and columns.
 * https://oeis.org/A202050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 33, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A202050
    Number of (n+2)×8 binary arrays avoiding patterns 001 and 110
    in rows and columns.

    Recurrence (for n > 9):
        a(n) =  9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4)
                +126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9)

    Initial values:
        a(1) = 1392,  a(2) = 5216,  a(3) = 15760,  a(4) = 41088,
        a(5) = 95984, a(6) = 205792, a(7) = 411696, a(8) = 777760,
        a(9) = 1400080
*/

#include <stdlib.h>   /* malloc, realloc, free */
#include <stddef.h>   /* size_t */

seq_t a(idx_t n)
{
    /* Memoised cache of already computed terms.
       cache[0] corresponds to a(1). */
    static seq_t *cache = NULL;
    static size_t sz = 0;   /* number of terms stored so far   */
    static size_t cap = 0;  /* capacity of the allocated array */

    if (n == 0) return 0;  /* undefined behaviour for n == 0, return 0 */

    /* Grow the cache until it contains at least n terms. */
    if ((size_t)n > sz)
    {
        size_t need = (size_t)n;

        if (need > cap)
        {
            size_t new_cap = (cap == 0) ? 16 : cap * 2;
            if (new_cap < need) new_cap = need;

            seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (new_cache == NULL)  /* allocation failure – abort */
                abort();
            cache = new_cache;
            cap   = new_cap;
        }

        /* Initial values a(1)…a(9). */
        static const seq_t init[9] = {
            1392, 5216, 15760, 41088, 95984,
            205792, 411696, 777760, 1400080
        };

        /* Compute missing terms. */
        while (sz < need)
        {
            if (sz < 9)
                cache[sz] = init[sz];   /* sz = 0 → a(1) */
            else
            {
                /* recurrence:
                     a(n) =  9*a(n-1) - 36*a(n-2) + 84*a(n-3)
                            - 126*a(n-4) +126*a(n-5) - 84*a(n-6)
                            + 36*a(n-7) - 9*a(n-8) + a(n-9)  */
                seq_t term =
                      9   * cache[sz-1]
                    + 84  * cache[sz-3]
                    + 126 * cache[sz-5]
                    +  36 * cache[sz-7]
                    +   cache[sz-9]
                    -   36 * cache[sz-2]
                    - 126 * cache[sz-4]
                    -   84 * cache[sz-6]
                    -    9 * cache[sz-8];

                cache[sz] = term;
            }
            ++sz;
        }
    }

    return cache[n-1];
}
