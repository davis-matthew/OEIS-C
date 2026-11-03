/*
 * A035872  Coordination sequence for A_46 lattice.
 * https://oeis.org/A035872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n = 0; a(n) = Σ_{d=1..46} C(47,d) * C(n-1,d-1) *
 *            C(46-d+n, n) for n ≥ 1
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 12 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    idx_t n_minus1 = n - 1;

    for (idx_t d = 1; d <= 46; ++d)
    {
        /* avoid unnecessary work if d-1 > n-1 */
        if (n_minus1 < d - 1)
            break;

        seq_t term = binomial(47, d);
        term = term * binomial(n_minus1, d - 1);
        term = term * binomial(46 - d + n, n);
        sum += term;
    }

    return sum;
}
