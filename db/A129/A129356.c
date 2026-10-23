/*
 * A129356  G.f.: A(x) = Product_{n>=1} [ (1-x)^3*(1 + 3x + 6x^2 +...+
 *          n(n+1)/2*x^(n-1)) ].
 * https://oeis.org/A129356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of xⁿ in ∏_{k≥1} [1 - (k+1)(k+2)/2·xᵏ +
 *            k(k+2)·xᵏ⁺¹ - k(k+1)/2·xᵏ⁺²]
 * Range:     i8 n <= 5, i16 n <= 14, i32 n <= 29, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    size_t sz = (size_t)(n + 1);
    seq_t *coeff = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!coeff)
        return (seq_t)0;              /* memory error – not expected */

    coeff[0] = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= n; ++i)
        coeff[i] = (seq_t)0;

    for (idx_t k = (idx_t)1; k <= n; ++k) {
        seq_t a_k = ((seq_t)(k + 1) * (k + 2)) / 2;   /* (k+1)(k+2)/2 */
        seq_t b_k = ((seq_t)k * (k + 2));            /* k(k+2)     */
        seq_t c_k = ((seq_t)k * (k + 1)) / 2;        /* k(k+1)/2   */

        /* Update coefficients in place, high to low */
        for (idx_t i = n; i > (idx_t)0; --i) {
            seq_t val = coeff[i];
            if (i >= k)
                val -= a_k * coeff[i - k];
            if (i >= k + (idx_t)1)
                val += b_k * coeff[i - (k + (idx_t)1)];
            if (i >= k + (idx_t)2)
                val -= c_k * coeff[i - (k + (idx_t)2)];
            coeff[i] = val;
        }
        /* coeff[0] stays unchanged (always 1) */
    }

    seq_t result = coeff[n];
    free(coeff);
    return result;
}
