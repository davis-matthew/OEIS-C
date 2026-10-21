/*
 * A071244  a(n) = n*(n-1)*(n^2 + 2)/6.
 * https://oeis.org/A071244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n^2 + 2)/6
 * Range:     u8 n <= 6, u16 n <= 25, u32 n <= 400, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t a1 = (seq_t)n;
    seq_t b1 = (seq_t)n - 1;
    seq_t c1 = a1 * a1 + 2;              /* n^2 + 2 */

    /* divide by 2 (product of two consecutive numbers is even) */
    if ((a1 & 1) == 0)
        a1 >>= 1;
    else
        b1 >>= 1;

    /* divide by 3 (at least one of n, n-1, n^2+2 is divisible by 3) */
    if (a1 % 3 == 0)
        a1 /= 3;
    else if (b1 % 3 == 0)
        b1 /= 3;
    else
        c1 /= 3;

    return a1 * b1 * c1;
}
