/*
 * A014087  Inverse of 78th cyclotomic polynomial.
 * https://oeis.org/A014087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the power series 1/Φ_78(x). It
 *            satisfies the recurrence a(n) = -∑_{i=1}^{24} φ_i · a(n-i)
 *            (n≥1), where (φ_0,φ_1,…,φ_24) are the coefficients of the 78‑th
 *            cyclotomic polynomial Φ_78(x). The sequence is periodic with
 *            period 78 (a(n+78)=a(n))
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   compute_phi78_coeffs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/compute_phi78_coeffs.h"

/* Entry point required by the test harness */
seq_t a(idx_t n)
{
    static bool ready = false;
    static seq_t coeff[25];   /* Φ_78 coefficients, φ_0..φ_24 */
    static seq_t period[78];  /* a(0)..a(77) */

    if (!ready) {
        /* compute Φ_78 coefficients once */
        compute_phi78_coeffs(coeff);

        /* generate the first 78 terms using the recurrence */
        period[0] = 1;
        for (int i = 1; i < 78; ++i) {
            seq_t s = 0;
            int limit = (i < 24) ? i : 24;
            for (int j = 1; j <= limit; ++j) {
                s += coeff[j] * period[i - j];
            }
            period[i] = -s;
        }
        ready = true;
    }
    return period[(size_t)(n % 78)];
}
