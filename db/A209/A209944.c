/*
 * A209944  a(n) is half the number of (n+1) X 2 0..2 arrays with every 2 X 2
 *          subblock having at most one duplicate clockwise edge difference.
 * https://oeis.org/A209944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 12*a(n-2) with a(1)=34, a(2)=258
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined below the offset, but harmless */
    if (n == 1)
        return 34;
    if (n == 2)
        return 258;

    seq_t a1 = 34;                /* a(1) */
    seq_t a2 = 258;               /* a(2) */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i)
    {
        an = (seq_t)6 * a2 + (seq_t)12 * a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
