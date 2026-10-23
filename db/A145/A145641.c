/*
 * A145641  Numbers whose binary representation is the concatenation of n 1's,
 *          n 0's and n 1's.
 * https://oeis.org/A145641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 8^n - 4^n + 2^n - 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute the value using the efficient definition:
       a(n) = (2^n - 1) * (2^(2n) + 1) = 8^n - 4^n + 2^n - 1      */
    seq_t pow2_n = pow2(n);     /* 2^n   */
    seq_t pow4_n = pow2(2 * n); /* 4^n   = 2^(2n) */
    seq_t pow8_n = pow2(3 * n); /* 8^n   = 2^(3n) */

    return pow8_n - pow4_n + pow2_n - 1;
}
