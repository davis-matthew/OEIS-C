/*
 * A027787  a(n) = 7*(n+1)*binomial(n+2,14).
 * https://oeis.org/A027787
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*(n+1)*binomial(n+2, 14)
 * Range:     u8 n <= 12, u16 n <= 14, u32 n <= 25, u64 n <= 94,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 12.  For n < 12 the value is
       undefined; we return 0 in that case. */
    if (n < 12)
        return 0;

    /* Compute binomial(n+2, 14) */
    seq_t bin = binomial(n + 2, 14);

    /* Compute the factor 7*(n+1) */
    seq_t factor = (seq_t)7 * ((seq_t)(n + 1));

    /* Final result */
    return bin * factor;
}
