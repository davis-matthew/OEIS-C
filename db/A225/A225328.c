/*
 * A225328  a(n) = A002426(n)^n, where A002426 is the central trinomial
 *          coefficients.
 * https://oeis.org/A225328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002426(n)^n where A002426(n) is the central * trinomial
 *            coefficient T(n) = Σ_{k=0}^{⌊n/2⌋} C(n,2k)·C(2k,k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 46 (2026-11-01)
 * Helpers:   binomial, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    /* Compute the central trinomial coefficient T(n). */
    seq_t t = (seq_t)0;
    idx_t m = n / 2;
    for (idx_t k = 0; k <= m; ++k)
    {
        seq_t c1 = binomial(n, 2 * k);
        seq_t c2 = binomial(2 * k, k);
        t = t + (c1 * c2);
    }
    /* Return T(n) raised to the n‑th power. */
    return ipow_idx(t, n);
}
