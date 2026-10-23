/*
 * A153339  Number of zig-zag paths from top to bottom of a rectangle of width
 *          5 with n rows whose color is that of the top right corner.
 * https://oeis.org/A153339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=3, a(2n)=4*3^(n-1), a(2n+1)=8*3^(n-1) (n>0)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 39, u64 n <= 80, u128 n <= 160,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 1)                      /* a(1)=3 */
        return (seq_t)3;

    /* exponent = floor(n/2) - 1 = (n>>1) - 1 */
    idx_t exp = (n >> 1) - 1;
    seq_t val = pow3(exp);           /* 3^exp */

    if ((n & 1u) == 0)               /* n even  ->  a(2n) */
        return val * (seq_t)4;
    else                              /* n odd   ->  a(2n+1) */
        return val * (seq_t)8;
}
