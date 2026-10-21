/*
 * A054338  8-fold convolution of A000302 (powers of 4).
 * https://oeis.org/A054338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+7,7) * 4^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 50,
 *            big any n
 * Helpers:   binom, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* Compute the binomial coefficient binom(n+7,7) */
    seq_t bin = binom(n + 7, (idx_t)7);

    /* Compute 4^n */
    seq_t p4 = pow4(n);

    /* Result is the product */
    return bin * p4;
}
