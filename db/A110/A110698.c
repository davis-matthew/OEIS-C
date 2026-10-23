/*
 * A110698  Kekulé numbers for certain benzenoids of trigonal symmetry.
 * https://oeis.org/A110698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20*binomial(2n,n)^3 - 18*binomial(2n,n)^2 +
 *            9*binomial(2n,n) - 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute C(2n, n) */
    seq_t c = binomial(2 * n, n);

    /* Compute powers of c */
    seq_t c2 = c * c;
    seq_t c3 = c2 * c;

    /* Apply the formula */
    seq_t result = (seq_t)20 * c3 - (seq_t)18 * c2 + (seq_t)9 * c - (seq_t)2;
    return result;
}
