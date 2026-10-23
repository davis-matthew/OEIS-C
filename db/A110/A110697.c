/*
 * A110697  Kekulé numbers for certain benzenoids of trigonal symmetry.
 * https://oeis.org/A110697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*binomial(2n, n)^3 - 12*binomial(2n, n)^2 +
 *            6*binomial(2n, n) - 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Compute the central binomial coefficient C(2n, n). */
    seq_t c = binom(2 * n, n);

    /* Square and cube the binomial. */
    seq_t c2 = c * c;    /* C(2n, n)^2 */
    seq_t c3 = c2 * c;   /* C(2n, n)^3 */

    /* Evaluate the polynomial: 9*c3 - 12*c2 + 6*c - 1. */
    return 9 * c3 - 12 * c2 + 6 * c - 1;
}
