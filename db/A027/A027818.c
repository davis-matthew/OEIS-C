/*
 * A027818  a(n) = (n+1)*binomial(n+6,6).
 * https://oeis.org/A027818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*binomial(n+6,6)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 57, u64 n <= 1443,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* compute binomial coefficient (n+6 choose 6) */
    seq_t coeff = binomial(n + 6, 6);
    /* multiply by (n+1) */
    coeff = coeff * ((seq_t)n + 1);
    return coeff;
}
