/*
 * A016417  469th cyclotomic polynomial.
 * https://oeis.org/A016417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of the 469th cyclotomic polynomial Φ_469(x)
 *            at x^n
 * Range:     i8 n <= 396, i16 n <= 396, i32 n <= 396, i64 n <= 396,
 *            i128 n <= 396, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdbool.h>  /* for bool */

/* Degree of the numerator and denominator polynomials used in the long division */
#define DEG_NUM   470          /* degree of (x^470 - x^469 - x + 1) */
#define DEG_DEN   74           /* degree of (x^74 - x^67 - x^7 + 1) */
#define DEG_OUT   396          /* degree of Φ_469(x) */

/* Cached coefficients of Φ_469(x) */
static seq_t coeffs[DEG_OUT + 1];
static bool coeffs_ready = false;

/* Compute the coefficients of Φ_469(x) once and store them in coeffs. */
static void compute_coeffs(void)
{
    seq_t num[DEG_NUM + 1] = {0};   /* N(x) = x^470 - x^469 - x + 1 */
    seq_t den[DEG_DEN + 1] = {0};   /* D(x) = x^74 - x^67 - x^7 + 1 */
    seq_t quot[DEG_OUT + 1] = {0};

    /* Coefficients of the numerator */
    num[DEG_NUM] = 1;
    num[DEG_NUM - 1] = -1;       /* coefficient of x^469 */
    num[1] = -1;                 /* coefficient of x^1  */
    num[0] = 1;                  /* constant term */

    /* Coefficients of the denominator */
    den[DEG_DEN] = 1;
    den[DEG_DEN - 7] = -1;       /* coefficient of x^67  */
    den[7] = -1;                 /* coefficient of x^7   */
    den[0] = 1;                  /* constant term */

    /* Long division of num by den to obtain the quotient = Φ_469(x) */
    for (int i = DEG_NUM; i >= DEG_DEN; --i) {
        seq_t c = num[i];
        if (c != 0) {
            int offset = i - DEG_DEN;          /* power at which the term appears in the quotient */
            quot[offset] = c;
            for (int j = 0; j <= DEG_DEN; ++j) {
                if (den[j] != 0) {
                    num[j + offset] -= c * den[j];
                }
            }
        }
    }

    /* Copy the quotient into the static cache */
    for (int i = 0; i <= DEG_OUT; ++i) {
        coeffs[i] = quot[i];
    }
    coeffs_ready = true;
}

/* Sequence function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!coeffs_ready) {
        compute_coeffs();
    }
    if (n > DEG_OUT) {
        return 0;            /* coefficients beyond the degree are zero */
    }
    return coeffs[(size_t)n];
}
