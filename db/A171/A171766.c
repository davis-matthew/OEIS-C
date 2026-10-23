/*
 * A171766  a(n) = 10^n*(3 + 7*10^n) + 7.
 * https://oeis.org/A171766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7*10^(2n) + 3*10^n + 7
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t t  = pow10(n);          /* 10^n   */
    seq_t t2 = pow10(n + n);      /* 10^(2n) */
    return (seq_t)7 * t2 + (seq_t)3 * t + (seq_t)7;
}
