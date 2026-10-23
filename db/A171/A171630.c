/*
 * A171630  a(n) = 10^n*(5+3*10^n)+3.
 * https://oeis.org/A171630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10^n*(5+3*10^n)+3
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t p10 = pow10(n);           /* 10^n */
    seq_t p10sq = p10 * p10;        /* 10^(2n) */
    seq_t term = (seq_t)5 * p10 + (seq_t)3 * p10sq + (seq_t)3;
    return term;
}
