/*
 * A035870  Coordination sequence for A_44 lattice.
 * https://oeis.org/A035870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 when n = 0, otherwise a(n) = Σ_{d=1..44} C(45,d) *
 *            C(n-1,d-1) * C(44-d+n,n)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    for (idx_t d = 1; d <= 44; ++d)
    {
        /* Skip terms where C(n-1,d-1)=0 */
        if (d - 1 > n - 1)
            break;

        seq_t term = binom(45, d);
        term *= binom(n - 1, d - 1);
        term *= binom(44 - d + n, n);
        sum += term;
    }
    return sum;
}
