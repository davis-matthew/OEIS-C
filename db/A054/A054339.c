/*
 * A054339  9-fold convolution of A000302 (powers of 4).
 * https://oeis.org/A054339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+8,8)*4^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 20, u128 n <= 48,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    seq_t binom = 1;
    for (idx_t i = 1; i <= 8; ++i) {
        seq_t term = (seq_t)n + (seq_t)i;
        binom = binom * term / (seq_t)i;
    }
    return binom * pow4(n);
}
