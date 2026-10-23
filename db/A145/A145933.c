/*
 * A145933  Expansion of chi(-q^6) * chi(-q^10) / (chi(-q) * chi(-q^15)) in
 *          powers of q where chi() is a Ramanujan theta function.
 * https://oeis.org/A145933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in ∏_{k≥1} (1 + q^k)(1 + q^(15k)) /
 *            ((1 + q^(6k))(1 + q^(10k))) Equivalent to ∏_{m≥1} (1 +
 *            q^m)^{e_m} where e_m = 1 + [15|m] - [6|m] - [10|m] (e_m ∈
 *            {0,1,2})
 * Range:     u8 n <= 35, u16 n <= 96, u32 n <= 294, u64 n <= 984,
 *            u128 n <= 3517, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate coefficient array, index 0..n */
    seq_t *coef = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!coef) return (seq_t)0;                /* fallback if allocation fails */

    /* Initialize all coefficients to 0; coeff[0] = 1 */
    for (idx_t i = 0; i <= n; ++i) coef[(size_t)i] = 0;
    coef[0] = 1;

    /* Build the product term‑by‑term. */
    for (idx_t m = 1; m <= n; ++m) {
        int e = 1;                           /* base exponent from (1+q^m) */
        if (m % 15 == 0) e += 1;             /* extra factor for (1+q^(15k)) */
        if (m % 6  == 0) e -= 1;             /* subtract factor for (1+q^(6k)) */
        if (m % 10 == 0) e -= 1;             /* subtract factor for (1+q^(10k)) */
        if (e == 0) continue;                /* factor cancels – do nothing */

        if (e == 1) {
            /* Multiply by (1 + q^m) */
            for (idx_t j = n; j >= m; --j) {
                coef[(size_t)j] += coef[(size_t)(j - m)];
            }
        } else { /* e == 2 */
            /* Multiply twice: results in (1 + q^m)^2 = 1 + 2q^m + q^(2m) */
            for (idx_t j = n; j >= m; --j) {
                coef[(size_t)j] += coef[(size_t)(j - m)];
            }
            for (idx_t j = n; j >= m; --j) {
                coef[(size_t)j] += coef[(size_t)(j - m)];
            }
        }
    }

    seq_t result = coef[(size_t)n];
    free(coef);
    return result;
}
