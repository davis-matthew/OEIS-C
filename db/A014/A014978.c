/*
 * A014978  Inverse of 969th cyclotomic polynomial.
 * https://oeis.org/A014978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) are the coefficients in the power‑series expansion of *
 *            1/Φ_969(x) (the inverse of the 969th cyclotomic polynomial). *
 *            The sequence is periodic with period 969. * * The implementation
 *            computes the recurrence from Φ_969(x), * then evaluates the
 *            terms modulo 969
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Maximum degree required during intermediate polynomial
 * calculations: 969 + 19 + 17 + 3 = 1008 < 1100
 */
#ifndef MAX_DEG
# define MAX_DEG 1100
#endif

/* Utility: initialise an array of seq_t to zero. */
static void zero_array(seq_t *arr, size_t n)
{
    for (size_t i = 0; i < n; ++i) arr[i] = 0;
}

/* Multiply two polynomials a (degree da) and b (degree db).
 * Result is written into res (size at least da+db+1) and
 * its degree into *dr.
 */
static void poly_mul(const seq_t *a, size_t da,
                     const seq_t *b, size_t db,
                     seq_t *res, size_t *dr)
{
    size_t len = da + db + 1;
    zero_array(res, len);
    for (size_t i = 0; i <= da; ++i) {
        if (a[i] == 0) continue;
        for (size_t j = 0; j <= db; ++j) {
            if (b[j] == 0) continue;
            res[i + j] += a[i] * b[j];
        }
    }
    *dr = len - 1;
}

/* Divide polynomial *poly (degree *deg) by (x^d – 1).
 * The division is exact.  The quotient replaces *poly
 * and its degree is updated in *deg.
 */
static void div_by_factor_xd_minus_1(seq_t *poly, size_t *deg, size_t d)
{
    if (d == 0) return;
    seq_t q[MAX_DEG + 1];
    zero_array(q, *deg - d + 1);

    for (long long i = (long long)*deg; i >= (long long)d; --i) {
        seq_t coeff = poly[i];
        if (coeff == 0) continue;
        seq_t *qptr = &q[i - d];
        *qptr += coeff;        /* quotient coefficient   */
        poly[i - d] += coeff;  /* cancel lower term       */
    }
    *deg -= d;

    /* copy quotient back to poly */
    for (size_t i = 0; i <= *deg; ++i) {
        poly[i] = q[i];
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t period[969];

    if (!initialized) {
        /* Build Φ_969(x) = (x^969−1)(x^19−1)(x^17−1)(x^3−1) /
         *            (x^323−1)(x^57−1)(x^51−1)(x−1)
         */
        seq_t poly[MAX_DEG + 1];
        seq_t factor[MAX_DEG + 1];
        seq_t tmp[MAX_DEG + 1];
        size_t deg = 0;

        zero_array(poly, MAX_DEG + 1);
        poly[0] = 1;           /* start with constant 1 */

        const size_t num_factors[] = { 969, 19, 17, 3 };
        for (size_t idx = 0; idx < sizeof num_factors / sizeof *num_factors; ++idx) {
            size_t d = num_factors[idx];
            zero_array(factor, MAX_DEG + 1);
            factor[0] = -1;
            factor[d] = 1;
            size_t new_deg;
            poly_mul(poly, deg, factor, d, tmp, &new_deg);
            /* copy back to poly */
            for (size_t i = 0; i <= new_deg; ++i) poly[i] = tmp[i];
            deg = new_deg;
        }

        const size_t den_factors[] = { 323, 57, 51, 1 };
        for (size_t idx = 0; idx < sizeof den_factors / sizeof *den_factors; ++idx) {
            size_t d = den_factors[idx];
            div_by_factor_xd_minus_1(poly, &deg, d);
        }
        /* After this, poly[0..deg] is Φ_969(x); deg should be 576. */

        /* Copy recurrence coefficients c(k) = φ_969[k] */
        size_t phi_deg = deg;                /* 576 */
        seq_t c[MAX_DEG + 1];
        for (size_t i = 0; i <= phi_deg; ++i) c[i] = poly[i];

        /* Compute first 969 terms using the recurrence:
         * a(0) = 1
         * a(n) = -Σ_{k=1..min(n,phi_deg)} c[k]*a(n-k)
         */
        seq_t a_seq[969];
        a_seq[0] = 1;
        for (size_t m = 1; m < 969; ++m) {
            seq_t sum = 0;
            size_t kmax = (m <= phi_deg) ? m : phi_deg;
            for (size_t k = 1; k <= kmax; ++k) {
                if (c[k] == 0) continue;
                sum += c[k] * a_seq[m - k];
            }
            a_seq[m] = -sum;
        }

        /* Store period values. */
        for (size_t i = 0; i < 969; ++i) period[i] = a_seq[i];
        initialized = true;
    }

    return period[(size_t)(n % 969)];
}
