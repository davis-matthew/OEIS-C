/*
 * A035874  Coordination sequence for A_48 lattice.
 * https://oeis.org/A035874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n >= 1, a(n) = Σ_{d=1..48} C(49,d) * C(n-1,d-1) *
 *            C(48-d+n,n) (OEIS A035874)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t sum = 0;
    idx_t n_1 = n - 1;

    for (idx_t d = 1; d <= 48; ++d)
    {
        /* If the second binomial would have a negative top, skip it. */
        if (d - 1 > n_1)
            continue;

        seq_t term = binomial(49, d);
        term *= binomial(n_1, d - 1);
        term *= binomial(48 - d + n, n);
        sum += term;
    }

    return sum;
}
