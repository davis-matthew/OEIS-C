/*
 * A014926  a(1)=1, a(n) = n*11^(n-1) + a(n-1).
 * https://oeis.org/A014926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23*a(n-1) - 143*a(n-2) + 121*a(n-3) with a(1)=1, a(2)=23,
 *            a(3)=386. This recurrence is derived from the closed‑form a(n) =
 *            (11^n*(10*n-1)+1)/100 and allows computation in O(n) time
 *            without ever evaluating 11^n directly
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for n < 1 */
        return 0;

    /* handle small indices before recursion */
    if (n == 1) return 1;
    if (n == 2) return 23;
    if (n == 3) return 386;

    /* keep a dynamic cache of computed values */
    static seq_t *cache = NULL;          /* cache[0] = a(1), cache[1] = a(2), … */
    static idx_t cached = 0;             /* number of values stored in cache */

    /* if needed, grow or initialise the cache */
    if (n > cached) {
        size_t new_cap = (size_t)n;       /* realloc() wants a size_t */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0)          /* allocation failed */
            return 0;

        /* first time initialisation ------------------------------------------------- */
        if (cached == 0) {
            tmp[0] = 1;     /* a(1) */
            tmp[1] = 23;    /* a(2) */
            tmp[2] = 386;   /* a(3) */
            cached = 3;     /* we now have 3 values stored            */
            /* compute values up to n */
            for (idx_t i = 4; i <= n; ++i)
                tmp[i-1] = 23 * tmp[i-2] - 143 * tmp[i-3] + 121 * tmp[i-4];
            cached = n;
        } else {
            /* extend cache from cached+1 up to n */
            for (idx_t i = cached + 1; i <= n; ++i)
                tmp[i-1] = 23 * tmp[i-2] - 143 * tmp[i-3] + 121 * tmp[i-4];
            cached = n;
        }
        cache = tmp;
    }

    return cache[n-1];
}
