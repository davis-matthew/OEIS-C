/*
 * A187029  Number of 4-step one or two collinear space at a time queen's
 *          tours on an n X n board summed over all starting positions.
 * https://oeis.org/A187029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   for n > 5, a(n) = 3504*n^2 - 18540*n + 24444 For n = 1..5 the
 *            values are: 0, 24, 1344, 7056, 19568
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Small n – use the exact known values */
    if (n <= 5)
    {
        /* values[0] is unused to keep indexing natural */
        static const seq_t values[6] = {0, 0, 24, 1344, 7056, 19568};
        return values[(size_t)n];
    }

    /* n > 5 – evaluate the quadratic formula */
    seq_t nn = (seq_t)n;
    seq_t t1 = (seq_t)3504 * nn * nn;     /* 3504*n^2      */
    seq_t t2 = (seq_t)18540 * nn;         /* 18540*n       */
    seq_t res = t1 - t2 + (seq_t)24444;   /* +24444        */
    return res;
}
