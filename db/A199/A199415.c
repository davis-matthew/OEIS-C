/*
 * A199415  a(n) = 11*6^n+1.
 * https://oeis.org/A199415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*6^n+1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow = big_pow((seq_t)6, (seq_t)n);
    return ((seq_t)11 * pow) + (seq_t)1;
}
