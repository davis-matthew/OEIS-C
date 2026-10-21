/*
 * A053131  Binomial coefficients C(2*n-8,9).
 * https://oeis.org/A053131
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n-8, 9)
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 25, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

/* Compute the A053131 term for index n.
   For n < 9 the sequence is defined as 0. */
seq_t a(idx_t n)
{
    if (n < 9) {
        return 0;
    }
    /* Compute 2*n-8 without overflow: 2*n is (n << 1). */
    idx_t NN = (n << 1) - 8;       /* 2*n - 8 */
    /* Use the provided binomial helper for small k = 9. */
    return binom(NN, 9);
}
