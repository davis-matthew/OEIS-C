/*
 * A171248  a(n) = 10^n*(2+3*10^n)+3.
 * https://oeis.org/A171248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10^n*(2+3*10^n)+3
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t p  = pow10(n);         /* 10^n   */
    seq_t p2 = pow10(n + n);     /* 10^(2n) */

    seq_t result = (seq_t)3 * p2 + (seq_t)2 * p + (seq_t)3;
    return result;
}
