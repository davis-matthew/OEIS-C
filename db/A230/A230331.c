/*
 * A230331  Number of n X 2 0..2 arrays x(i,j) with each element horizontally,
 *          vertically or antidiagonally next to at least one element with
 *          value (x(i,j)+1) mod 3, and upper left element zero.
 * https://oeis.org/A230331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence: a(n) = 5*a(n-1) + 8*a(n-2) + 4*a(n-3),
   with a(1)=0, a(2)=6, a(3)=34.  The sequence is defined for n ≥ 1. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed values.
       cache[0] is unused; cache[i] holds a(i).  */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;   /* capacity in array elements (≥ 1) */
    static idx_t   last   = 0;   /* highest index computed (≤ cap-1) */

    if (n == 0) {                /* undefined offset – return 0 */
        return (seq_t)0;
    }

    /* Ensure the cache is large enough to store indices up to n. */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) abort();       /* allocation failure */
        if (cache) {
            /* copy existing values */
            for (idx_t i = 1; i <= last; ++i)
                tmp[i] = cache[i];
            free(cache);
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute new values up to n.  */
    if (last < n) {
        for (idx_t i = last + 1; i <= n; ++i) {
            if (i == 1) {
                cache[i] = (seq_t)0;
            } else if (i == 2) {
                cache[i] = (seq_t)6;
            } else if (i == 3) {
                cache[i] = (seq_t)34;
            } else {
                cache[i] = 5 * cache[i - 1] + 8 * cache[i - 2] + 4 * cache[i - 3];
            }
        }
        last = n;
    }

    return cache[n];
}
