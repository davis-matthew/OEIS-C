/*
 * A016408  429th cyclotomic polynomial.
 * https://oeis.org/A016408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient n of the 429th cyclotomic polynomial Φ₄₂₉(x)
 * Range:     i8 n <= 240, i16 n <= 240, i32 n <= 240, i64 n <= 240,
 *            i128 n <= 240, big any n
 * Helpers:   poly_mul_out, poly_xk_minus1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/poly_mul_out.h"
#include "../utils/poly_xk_minus1.h"

/* Divide dividend (degree N) by monic divisor (degree D).
   The quotient is stored in quotient (array size N-D+1).  */
static void poly_div_monic(const seq_t *dividend, size_t N,
                           const seq_t *divisor, size_t D,
                           seq_t *quotient, size_t *qdeg)
{
    seq_t rem[457];
    for (size_t i = 0; i <= N; ++i) rem[i] = dividend[i];
    for (size_t i = 0; i <= N - D; ++i) quotient[i] = 0;

    for (int i = (int)N; i >= (int)D; --i) {
        seq_t coef = rem[i];
        if (coef != 0) {
            size_t j = (size_t)(i - D);
            quotient[j] = coef;
            for (size_t k = 0; k <= D; ++k) {
                rem[j + k] -= coef * divisor[k];
            }
        }
    }
    *qdeg = N - D;
    while (*qdeg > 0 && quotient[*qdeg] == 0) (*qdeg)--;
}

/* Compute the coefficients of Φ₄₂₉(x) and store them in coeff_arr[0..240]. */
static void compute_coeffs(seq_t coeff_arr[241])
{
    seq_t num[457], den[457], fac[457], temp[457];
    size_t num_deg, den_deg, fac_deg, temp_deg;

    /* -------- numerator: (x^429-1)(x^13-1)(x^11-1)(x^3-1) -------- */
    poly_xk_minus1(429, num, &num_deg);
    poly_xk_minus1(13, fac, &fac_deg);
    poly_mul_out(num, num_deg, fac, fac_deg, temp, &num_deg);
    for (size_t i = 0; i <= num_deg; ++i) num[i] = temp[i];

    poly_xk_minus1(11, fac, &fac_deg);
    poly_mul_out(num, num_deg, fac, fac_deg, temp, &num_deg);
    for (size_t i = 0; i <= num_deg; ++i) num[i] = temp[i];

    poly_xk_minus1(3, fac, &fac_deg);
    poly_mul_out(num, num_deg, fac, fac_deg, temp, &num_deg);
    for (size_t i = 0; i <= num_deg; ++i) num[i] = temp[i];

    /* -------- denominator: (x^143-1)(x^39-1)(x^33-1)(x^1-1) -------- */
    poly_xk_minus1(143, den, &den_deg);
    poly_xk_minus1(39, fac, &fac_deg);
    poly_mul_out(den, den_deg, fac, fac_deg, temp, &den_deg);
    for (size_t i = 0; i <= den_deg; ++i) den[i] = temp[i];

    poly_xk_minus1(33, fac, &fac_deg);
    poly_mul_out(den, den_deg, fac, fac_deg, temp, &den_deg);
    for (size_t i = 0; i <= den_deg; ++i) den[i] = temp[i];

    poly_xk_minus1(1, fac, &fac_deg);
    poly_mul_out(den, den_deg, fac, fac_deg, temp, &den_deg);
    for (size_t i = 0; i <= den_deg; ++i) den[i] = temp[i];

    /* -------- division: num / den -------- */
    seq_t quot[241];
    size_t qdeg;
    poly_div_monic(num, num_deg, den, den_deg, quot, &qdeg);
    for (size_t i = 0; i <= qdeg; ++i) coeff_arr[i] = quot[i];
    /* fill remaining entries with 0 if any */
    for (size_t i = qdeg + 1; i <= 240; ++i) coeff_arr[i] = 0;
}

seq_t a(idx_t n)
{
    static bool ready = false;
    static seq_t coeff[241];
    if (!ready) {
        compute_coeffs(coeff);
        ready = true;
    }
    if (n > 240) return 0;
    return coeff[n];
}
