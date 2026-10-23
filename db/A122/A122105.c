/*
 * A122105  Sum of the bottom levels of all columns in all deco polyominoes of
 *          height n. A deco polyomino is a directed column-convex polyomino
 *          in which the height, measured along the diagonal, is attained only
 *          in the last column.
 * https://oeis.org/A122105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n-1)*a(n-1) - (n-1)^2*a(n-2) + (n-2)!*(n-2) with
 *            initial values a(0)=a(1)=a(2)=0. This implements the recurrence,
 *            computing factorials as needed
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    seq_t a_prev2 = 0; /* a(n-2) */
    seq_t a_prev1 = 0; /* a(n-1) */
    seq_t a_curr  = 0;
    seq_t fact    = 1; /* (0)! for i=2 */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t B = fact * (seq_t)(i - 2);  /* (i-2)! * (i-2) */

        if (i >= 3)
        {
            /* use 128‑bit temporaries to avoid overflow on 64‑bit builds */
            unsigned __int128 t1 = (unsigned __int128)(2*i - 1)
                                 * (unsigned __int128)a_prev1;
            unsigned __int128 t2 = (unsigned __int128)(i - 1)
                                 * (unsigned __int128)(i - 1)
                                 * (unsigned __int128)a_prev2;
            unsigned __int128 t3 = (unsigned __int128)B;
            a_curr = (seq_t)(t1 - t2 + t3);
        }
        else
        {
            a_curr = 0;
        }

        a_prev2 = a_prev1;
        a_prev1 = a_curr;
        fact    = fact * (seq_t)(i - 1);   /* prepare (i-1)! for next step */
    }

    return a_curr;
}
