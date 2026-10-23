/*
 * A183678  Number of (n+1) X 6 0..7 arrays with every 2 X 2 subblock summing
 *          to 14.
 * https://oeis.org/A183678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*8^n + 434*7^n + 3612*6^n + 13510*5^n + 32648*4^n +
 *            58506*3^n + 79924*2^n + 73502
 * Range:     u8 none, u16 none, u32 n <= 7, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t pow2 = pow_uint(2, n);
    seq_t pow3 = pow_uint(3, n);
    seq_t pow4 = pow_uint(4, n);
    seq_t pow5 = pow_uint(5, n);
    seq_t pow6 = pow_uint(6, n);
    seq_t pow7 = pow_uint(7, n);
    seq_t pow8 = pow_uint(8, n);

    seq_t result = 73502;
    result += 79924 * pow2;
    result += 58506 * pow3;
    result += 32648 * pow4;
    result += 13510 * pow5;
    result += 3612 * pow6;
    result += 434 * pow7;
    result += 8 * pow8;
    return result;
}
