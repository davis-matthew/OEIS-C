/*
 * A155668  a(n) = 11^n-8^n+1.
 * https://oeis.org/A155668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n - 8^n + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow11 = big_pow((seq_t)11, (seq_t)n);
    seq_t pow8  = big_pow((seq_t)8, (seq_t)n);
    return pow11 - pow8 + (seq_t)1;
}
