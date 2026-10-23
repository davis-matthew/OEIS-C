/*
 * A154407  a(n) = 5*2^(n-1) + 3*6^n/2.
 * https://oeis.org/A154407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*2^n + 3*6^n)/2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* compute 5*2^n + 3*6^n and divide by 2 */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);
    seq_t pow6 = big_pow((seq_t)6, (seq_t)n);
    seq_t val  = (5 * pow2 + 3 * pow6) / 2;
    return val;
}
