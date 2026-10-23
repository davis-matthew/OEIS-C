/*
 * A035871  Coordination sequence for A_45 lattice.
 * https://oeis.org/A035871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d=1..45} C(46,d) * C(n-1,d-1) * C(45-d+n, n) where n
 *            = m/2 is the index (norm m is always even)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    idx_t dmax = (n < 45) ? n : 45;

    for (idx_t d = 1; d <= dmax; ++d) {
        seq_t c46  = binomial(46, d);
        seq_t c1   = binomial(n - 1, d - 1);           /* C(n-1,d-1) */
        if (!c1)
            continue;                                 /* zero term */

        seq_t c2   = binomial(45 - d + n, n);        /* C(45-d+n, n) */
        if (!c2)
            continue;

        seq_t term = c46;
        term *= c1;
        term *= c2;
        sum += term;
    }

    return sum;
}
