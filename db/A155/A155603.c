/*
 * A155603  a(n) = 5^n+3^n-1.
 * https://oeis.org/A155603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n+3^n-1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t exp = (seq_t)n;
    seq_t pow5 = big_pow((seq_t)5, exp);
    seq_t pow3 = big_pow((seq_t)3, exp);
    return pow5 + pow3 - (seq_t)1;
}
