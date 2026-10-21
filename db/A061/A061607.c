/*
 * A061607  Ooguri-Vafa invariants of disk domain wall degeneracies for brane
 *          I in the O(K) -> P^1 X P^1 geometry.
 * https://oeis.org/A061607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 9, u64 n <= 47, u128 n <= 200,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the OEIS formula
 *   a(n) = (n+1) * Σ_{k=0..6} h(k) * binomial(n - k + 16, 16)
 * where h = (1, 53, 476, 1044, 476, 53, 1).
 * The routine uses the binom() helper from "../utils/binom.h".
 * It handles all n ≥ 0 and avoids unsigned underflow by
 * only evaluating terms when n ≥ k.  The calculation keeps
 * intermediate values in seq_t so it works with any
 * arbitrary‑precision or fixed‑size integer type supplied
 * by the build system.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* h(k) coefficients */
    static const seq_t h[7] = {
        (seq_t)1,
        (seq_t)53,
        (seq_t)476,
        (seq_t)1044,
        (seq_t)476,
        (seq_t)53,
        (seq_t)1
    };

    seq_t sum = 0;
    for (idx_t k = 0; k < 7; ++k) {
        if (n < k) continue;                    /* term would be zero */
        /* Compute binomial(n - k + 16, 16) */
        seq_t bin = binom(n - k + 16U, 16);
        sum += h[k] * bin;
    }
    return ((seq_t)n + 1) * sum;
}
