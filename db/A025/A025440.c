/*
 * A025440  Expansion of 1/((1-2x)(1-3x)(1-4x)(1-6x)).
 * https://oeis.org/A025440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -2^n + 9*3^n - 16*4^n + 9*6^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    seq_t pow4 = pow2 * pow2;
    seq_t pow6 = pow2 * pow3;

    seq_t res = pow3 * (seq_t)9;
    res = res + pow6 * (seq_t)9;
    res = res - pow2;
    res = res - pow4 * (seq_t)16;

    return res;
}
