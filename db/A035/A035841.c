/*
 * A035841  Coordination sequence for A_15 lattice.
 * https://oeis.org/A035841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 0; a(n) = Σ_{d=1..15} C(16,d) * C(n-1,d-1) *
 *            C(15-d+n,n) for n ≥ 1 where C is the binomial coefficient
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 37, u128 n <= 888,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* precompute binom(16,d) for d = 1..15 once */
    static seq_t comb16[16];
    static bool initialized = false;
    if (!initialized) {
        for (idx_t d = 1; d <= 15; ++d)
            comb16[d] = binomial(16, d);
        initialized = true;
    }

    seq_t sum = 0;
    for (idx_t d = 1; d <= 15; ++d) {
        /* C(n-1,d-1) is zero for d-1 > n-1, so skip those */
        if (n - 1 < d - 1)
            continue;
        seq_t term = comb16[d];
        term *= binomial(n - 1, d - 1);
        term *= binomial(15 - d + n, n);
        sum += term;
    }
    return sum;
}
