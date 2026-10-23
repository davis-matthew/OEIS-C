/*
 * A125910  a(n) is the number of nonnegative integers k less than 10^n such
 *          that the decimal representation of k lacks the digit 1, at least
 *          one of digits 2,3,4 and at least one of digits 5,6,7,8,9.
 * https://oeis.org/A125910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 15*7^n-45*6^n+65*5^n-55*4^n+28*3^n-8*2^n+1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* only for completeness; offset is 1 */
    seq_t pow2 = pow_uint(2, n);
    seq_t pow3 = pow_uint(3, n);
    seq_t pow4 = pow_uint(4, n);
    seq_t pow5 = pow_uint(5, n);
    seq_t pow6 = pow_uint(6, n);
    seq_t pow7 = pow_uint(7, n);

    seq_t result = (seq_t)15 * pow7
                 - (seq_t)45 * pow6
                 + (seq_t)65 * pow5
                 - (seq_t)55 * pow4
                 + (seq_t)28 * pow3
                 - (seq_t)8  * pow2
                 + (seq_t)1;

    return result;
}
