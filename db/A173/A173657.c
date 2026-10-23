/*
 * A173657  a(n) = 2+2^n+3^n.
 * https://oeis.org/A173657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + 2^n + 3^n
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute 2^n, 3^n and sum with 2 */
    seq_t pow2 = pow_uint(2, n);
    seq_t pow3 = pow_uint(3, n);
    return pow2 + pow3 + (seq_t)2;
}
