/*
 * A121753  Number of deco polyominoes of height n in which all columns end at
 *          an odd level. A deco polyomino is a directed column-convex
 *          polyomino in which the height, measured along the diagonal, is
 *          attained only in the last column.
 * https://oeis.org/A121753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = (1 + 2*floor((n-2)/2)) * a(n-1) -
                       (floor((n-1)/2)*floor((n-2)/2)-1) * a(n-2);
   a(1) = 1, a(2) = 1. */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1 || n == 2)
        return 1;

    /* a(1) and a(2) are both 1. */
    seq_t prev2 = 1; /* a(i-2)   */
    seq_t prev1 = 1; /* a(i-1)   */
    seq_t cur   = 0; /* a(i)     */

    for (idx_t i = 3; i <= n; ++i)
    {
        idx_t f1 = (i - 2) / 2; /* floor((i-2)/2) */
        idx_t f2 = (i - 1) / 2; /* floor((i-1)/2) */

        seq_t coeff = 1 + 2 * (seq_t)f1;   /* 1+2*floor((i-2)/2) */
        seq_t term1 = prev1 * coeff;        /* coeff * a(i-1)   */

        seq_t prod   = (seq_t)f1 * (seq_t)f2;/* floor((i-1)/2)*floor((i-2)/2) */

        /* a(i) = coeff*a(i-1) + a(i-2) - prod*a(i-2) */
        cur = term1 + prev2;
        if (prod != 0)
            cur -= prev2 * prod;

        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
