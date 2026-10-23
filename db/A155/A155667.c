/*
 * A155667  a(n) = 10^n-8^n+1.
 * https://oeis.org/A155667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n - 8^n + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t p10 = big_pow((seq_t)10, (seq_t)n);
    seq_t p8  = big_pow((seq_t)8,  (seq_t)n);
    return p10 - p8 + (seq_t)1;
}
