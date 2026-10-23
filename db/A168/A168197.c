/*
 * A168197  a(n) = 3*n - a(n-1) + 1 with n > 1, a(1)=0.
 * https://oeis.org/A168197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 5 + 11*(-1)^n)/4
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /*  a(n) = (6*n + 5 + 11*(-1)^n)/4 ;
        For unsigned arithmetic we add or subtract 11
        depending on the parity of n. */
    seq_t temp = (seq_t)6 * n + (seq_t)5;
    if ((n & 1) == 0)           /* n even: add 11  */
        temp += (seq_t)11;
    else                        /* n odd : subtract 11 */
        temp -= (seq_t)11;
    return temp / (seq_t)4;
}
