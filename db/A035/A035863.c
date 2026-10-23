/*
 * A035863  Coordination sequence for A_37 lattice.
 * https://oeis.org/A035863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1, if n = 0; a(n) = Σ_{d=1..37}
 *            C(38,d)·C(n-1,d-1)·C(37-d+n,n) for n ≥ 1
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the helper that declares binomial() */
#include "../utils/binomial.h"
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Special case n = 0. */
    if (n == 0)
        return (seq_t)1;

    /* Pre‑compute the fixed binomial coefficients C(38,d).  D 1..37. */
    static seq_t binom38[38] = {0};
    static bool binom38_init = false;
    if (!binom38_init) {
        for (idx_t d = 1; d <= 37; ++d)
            binom38[d] = binomial(38, d);
        binom38_init = true;
    }

    seq_t result = 0;
    for (idx_t d = 1; d <= 37; ++d) {
        seq_t term = binom38[d];
        term = term * binomial(n - 1, d - 1);
        term = term * binomial(37 - d + n, n);
        result += term;
    }
    return result;
}
