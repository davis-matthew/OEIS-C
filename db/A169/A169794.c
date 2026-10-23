/*
 * A169794  Expansion of ((1-x)/(1-2*x))^7.
 * https://oeis.org/A169794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{j=0}^{min(7,n)} (-1)^j · C(7,j) · C(n-j+6,6) · 2^(n-j)
 *            (coefficients of the expansion of ((1-x)/(1-2x))^7)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 45, u128 n <= 103,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Pre‑computed values of C(7,j) */
    static const seq_t comb7[8] = {1, 7, 21, 35, 35, 21, 7, 1};

    seq_t pos = 0;          /* sum of positive terms   */
    seq_t neg = 0;          /* sum of negative terms   */

    idx_t maxj = 7;
    if (n < maxj) maxj = n; /* only j ≤ n contribute */

    for (idx_t j = 0; j <= maxj; ++j) {
        /* C(n-j+6,6) – the binomial with fixed k=6 */
        idx_t N = n - j + 6;
        seq_t bin_n = binom(N, 6);

        /* term = C(7,j) * C(n-j+6,6) * 2^(n-j) */
        seq_t term = comb7[j] * bin_n * pow2(n - j);

        if ((j & 1) == 0)  /* j even → positive sign   */
            pos += term;
        else               /* j odd  → negative sign   */
            neg += term;
    }
    return pos - neg;      /* pos ≥ neg for all n ≥ 0 */
}
