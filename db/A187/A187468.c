/*
 * A187468  Sum of the squares modulo 2^n of the odd numbers less than 2^n.
 * https://oeis.org/A187468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 2^(n-2) * (2^n - 6) for n >= 3 (with a(1) = 1, a(2) = 2)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    seq_t two_n          = pow2(n);      /* 2^n */
    seq_t two_n_minus_2  = pow2(n-2);    /* 2^(n-2) */
    return two_n_minus_2 * (two_n - (seq_t)6);
}
