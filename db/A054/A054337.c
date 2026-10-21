/*
 * A054337  7-fold convolution of A000302 (powers of 4).
 * https://oeis.org/A054337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+6, 6) * 4^n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 22, u128 n <= 51,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* Compute C(n+6,6) multiplicatively.
       The intermediate division is always exact on each step. */
    seq_t binom = 1;
    for (idx_t i = 1; i <= 6; ++i) {
        binom *= (seq_t)(n + i);
        binom /= (seq_t)i;
    }

    /* Compute 4^n via the provided utility function. */
    seq_t pow4n = pow4(n);

    return binom * pow4n;
}
