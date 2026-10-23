/*
 * A178294  Number of collinear point triples in a 4 X 4 X 4 X...
 *          n-dimensional cubic grid.
 * https://oeis.org/A178294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8^n/2 - 3*4^n/2 + 6^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t pow8_half = big_pow((seq_t)8, (seq_t)n) / (seq_t)2;
    seq_t pow4_half = big_pow((seq_t)4, (seq_t)n) / (seq_t)2;
    seq_t pow6      = big_pow((seq_t)6, (seq_t)n);

    return pow8_half - (seq_t)3 * pow4_half + pow6;
}
