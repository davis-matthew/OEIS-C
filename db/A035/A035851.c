/*
 * A035851  Coordination sequence for A_25 lattice.
 * https://oeis.org/A035851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = Σ_{d=1}^{25} C(26,d) * C(n‑1,d‑1) * C(25‑d+n,n)
 *            for n ≥ 1
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = 0;
    idx_t d;

    for (d = 1; d <= 25; ++d) {
        seq_t c1 = binomial(26, d);
        seq_t c2 = binomial(n - 1, d - 1);
        seq_t c3 = binomial(25 - d + n, n);

        seq_t prod = c1 * c2;
        prod = prod * c3;

        result += prod;
    }

    return result;
}
