/*
 * A178299  Number of collinear point 4-tuples in a 6 X 6 X 6 X...
 *          n-dimensional cubic grid.
 * https://oeis.org/A178299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 3*8^n + (3*10^n + 12^n)/2 - 5*6^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* compute powers using the helper big_pow */
    seq_t p8   = big_pow((seq_t)8,   (seq_t)n);
    seq_t p10  = big_pow((seq_t)10,  (seq_t)n);
    seq_t p6   = big_pow((seq_t)6,   (seq_t)n);
    seq_t p12  = big_pow((seq_t)12,  (seq_t)n);

    seq_t term1 = p8 * (seq_t)3;
    seq_t term2 = (p10 * (seq_t)3 + p12) / (seq_t)2;
    seq_t term3 = p6 * (seq_t)5;

    return term1 + term2 - term3;
}
