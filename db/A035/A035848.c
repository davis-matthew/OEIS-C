/*
 * A035848  Coordination sequence for A_22 lattice.
 * https://oeis.org/A035848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n = 0; a(n) = Σ_{d=1..22} C(23, d) * C(n-1, d-1) *
 *            C(22-d+n, n) for n ≥ 1 (here C(a,b) is the binomial coefficient)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t total = 0;
    for (idx_t d = 1; d <= 22; ++d) {
        seq_t c1 = binomial(23, d);
        if (n - 1 < d - 1) continue;          /* binomial( n-1, d-1 ) = 0 */
        seq_t c2 = binomial(n - 1, d - 1);
        seq_t c3 = binomial(n + 22 - d, n);
        seq_t term = c1;
        term *= c2;
        term *= c3;
        total += term;
    }
    return total;
}
