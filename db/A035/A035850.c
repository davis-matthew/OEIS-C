/*
 * A035850  Coordination sequence for A_24 lattice.
 * https://oeis.org/A035850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{d=1..24} C(25,d) * C(n-1,d-1) * C(24-d+n, n), with
 *            a(0) = 1 (m = 0 case)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t total = 0;
    for (idx_t d = 1; d <= 24 && d <= n; ++d)
    {
        seq_t term = binomial(25, d);         /* C(25, d)   */
        term *= binomial(n - 1, d - 1);       /* C(n-1, d-1)*/
        term *= binomial(24 - d + n, n);     /* C(24-d+n, n)*/
        total += term;
    }
    return total;
}
