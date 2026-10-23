/*
 * A125908  a(n) is the number of nonnegative integers k less than 10^n such
 *          that the decimal representation of k lacks the digits 1 and 2 and
 *          at least one of digits 3,4,5,6,7,8,9.
 * https://oeis.org/A125908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 7*7^n-21*6^n+35*5^n-35*4^n+21*3^n-7*2^n+1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* If n == 0 the sequence is undefined by offset 1; return 0. */
    if (n == 0) return 0;

    seq_t p7 = pow_uint(7, n);
    seq_t p6 = pow_uint(6, n);
    seq_t p5 = pow_uint(5, n);
    seq_t p4 = pow_uint(4, n);
    seq_t p3 = pow_uint(3, n);
    seq_t p2 = pow_uint(2, n);

    seq_t result = (seq_t)7 * p7;
    result -= (seq_t)21 * p6;
    result += (seq_t)35 * p5;
    result -= (seq_t)35 * p4;
    result += (seq_t)21 * p3;
    result -= (seq_t)7  * p2;
    result += (seq_t)1;
    return result;
}
