/*
 * A015838  Inverse of 1829th cyclotomic polynomial.
 * https://oeis.org/A015838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the formal power series 1/Φ₁₈₂₉(x),
 *            where Φ₁₈₂₉(x) is the 1829‑th cyclotomic polynomial. The
 *            sequence is periodic with period 1829
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Coefficients of Φ₁₈₂₉(x), degree 1740. */
    static seq_t phi[1741];
    /* First 1829 terms of the reciprocal series. */
    static seq_t seq[1829];
    static bool init = false;

    if (!init) {
        /* Φ₁₈₂₉(x) = (x-1)(x²³⁰⁹-1)/((x³¹-1)(x⁵⁹-1))
           = 1 - x³¹ - x⁵⁹ + x⁹⁰.
           Denominator coefficients:
           d₀ = 1, d₃₁ = -1, d₅₉ = -1, d₉₀ = 1.           */
        for (idx_t k = 0; k <= 1740; ++k) {
            seq_t Pk = 0;                     /* Coefficient of P(x) = (x-1)(x²³⁰⁹-1) */
            if (k == 0)       Pk =  1;
            else if (k == 1)  Pk = -1;
            /* terms at 1829 and 1830 are beyond the range we need for φ. */

            seq_t sum = 0;
            if (k >= 31) sum += (-1) * phi[k - 31];
            if (k >= 59) sum += (-1) * phi[k - 59];
            if (k >= 90) sum += ( 1) * phi[k - 90];
            phi[k] = Pk - sum;               /* Solve P = D · Φ  for Φ coefficients */
        }

        /* Compute the reciprocal series coefficients a(n).               */
        seq[0] = 1;
        for (idx_t k = 1; k < 1829; ++k) {
            seq_t sum = 0;
            idx_t max_i = k < 1740 ? k : 1740;
            for (idx_t i = 1; i <= max_i; ++i) {
                sum += phi[i] * seq[k - i];
            }
            seq[k] = -sum;                   /* a_k = - Σ_{i=1}^{1740} φ_i · a_{k-i} */
        }

        init = true;
    }
    return seq[n % 1829];
}
