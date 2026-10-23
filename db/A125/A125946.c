/*
 * A125946  a(n) is the number of nonnegative integers k less than 10^n such
 *          that the decimal representation of k lacks at least one of digits
 *          1,2, at least one of digits 3,4,5 and at least one of digits
 *          6,7,8,9.
 * https://oeis.org/A125946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 24*7^n-72*6^n+98*5^n-76*4^n+35*3^n-9*2^n+1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute the powers once to avoid repeated explications */
    seq_t p7 = pow_uint(7, n);
    seq_t p6 = pow_uint(6, n);
    seq_t p5 = pow_uint(5, n);
    seq_t p4 = pow_uint(4, n);
    seq_t p3 = pow_uint(3, n);
    seq_t p2 = pow_uint(2, n);

    /* Positive part of the expression */
    seq_t sum_pos = 24 * p7 + 98 * p5 + 35 * p3 + 1;

    /* Negative part of the expression */
    seq_t sum_neg = 72 * p6 + 76 * p4 + 9 * p2;

    /* The final result is the difference, guaranteed non‑negative. */
    return sum_pos - sum_neg;
}
