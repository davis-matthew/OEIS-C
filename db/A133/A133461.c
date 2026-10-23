/*
 * A133461  a(n) = 4^n*3^(n^2).
 * https://oeis.org/A133461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n * 3^(n^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* 4^n */
    seq_t pow4 = pow_uint(4, n);

    /* 3^(n^2) */
    idx_t nsq = n * n;
    seq_t pow3 = pow_uint(3, nsq);

    /* product */
    return pow4 * pow3;
}
