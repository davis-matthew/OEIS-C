/*
 * A015043  Inverse of 1034th cyclotomic polynomial.
 * https://oeis.org/A015043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the coefficient sequence of 1/Φ₁₀₃₄(x), where Φ₁₀₃₄ is
 *            the 1034th cyclotomic polynomial. The sequence is periodic with
 *            period 1034
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   poly_div_monic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: compute Φ₁₀₃₄ as a polynomial with integer coefficients,
   then apply the linear recurrence a(0)=1,
   a(n)= -∑_{k=1}^{deg} Φ[k]·a(n-k)   (n>0).
   All values are stored modulo the period. */

#include <stddef.h>   /* for size_t */
#include "../utils/poly_div_monic.h"

static seq_t period[1034];
static int initialized = 0;

/* Multiply two polynomials a and b (degrees da and db) into result r.
   All polynomials are represented as arrays of length at least da+db+1. */
static void poly_mul(const seq_t *a, size_t da, const seq_t *b, size_t db,
                     seq_t *r)
{
    for (size_t i = 0; i <= da + db; ++i) r[i] = 0;
    for (size_t i = 0; i <= da; ++i) {
        if (a[i] == 0) continue;
        for (size_t j = 0; j <= db; ++j) {
            r[i + j] += a[i] * b[j];
        }
    }
}

/* Compute Φ₁₀₃₄(x) and the period sequence. */
static void compute_period(void)
{
    /* All divisors of 1034 in increasing order. */
    const int divisors[8] = {1, 2, 11, 22, 47, 94, 517, 1034};
    const size_t D = 8;

    /* Storage for the cyclotomic polynomials. */
    seq_t polys[8][1035];
    size_t degs[8];

    /* Compute Φ_d for each divisor d. */
    for (size_t i = 0; i < D; ++i) {
        int d = divisors[i];

        /* Numerator: x^d - 1. */
        seq_t num[1035];
        for (size_t j = 0; j <= d; ++j) num[j] = 0;
        num[0] = -1;
        num[d] = 1;

        /* Denominator: product of Φ_t for t | d, t < d. */
        seq_t denom[1035], temp[1035];
        size_t degDen = 0;
        for (size_t j = 0; j <= d; ++j) denom[j] = 0;
        denom[0] = 1;               /* monic */

        for (size_t j = 0; j < i; ++j) {
            int t = divisors[j];
            if (d % t == 0) {
                size_t degT = degs[j];
                /* temp = denom * Φ_t */
                for (size_t k = 0; k <= degDen + degT; ++k) temp[k] = 0;
                for (size_t k = 0; k <= degDen; ++k) {
                    if (denom[k] == 0) continue;
                    for (size_t l = 0; l <= degT; ++l) {
                        temp[k + l] += denom[k] * polys[j][l];
                    }
                }
                /* store back */
                for (size_t k = 0; k <= degDen + degT; ++k) denom[k] = temp[k];
                degDen += degT;
            }
        }

        /* Divide numerator by denominator (monic). */
        seq_t res[1035];
        size_t degRes = d - degDen;
        poly_div_monic(num, d, denom, degDen, res);

        /* Store polynomial Φ_d. */
        for (size_t j = 0; j <= degRes; ++j) polys[i][j] = res[j];
        degs[i] = degRes;           /* degs[7] will be φ(1034)=460 */
    }

    /* Extract Φ₁₀₃₄ coefficients. */
    seq_t phi[1035];
    for (size_t i = 0; i <= 1034; ++i) phi[i] = 0;
    size_t phi_deg = degs[7];
    for (size_t i = 0; i <= phi_deg; ++i) phi[i] = polys[7][i];

    /* Build the periodic sequence. */
    period[0] = 1;
    for (size_t n = 1; n < 1034; ++n) {
        seq_t sum = 0;
        size_t maxk = (n <= phi_deg) ? n : phi_deg;
        for (size_t k = 1; k <= maxk; ++k) {
            sum += phi[k] * period[n - k];
        }
        period[n] = -sum;
    }

    initialized = 1;
}

seq_t a(idx_t n)
{
    if (!initialized) compute_period();
    size_t idx = (size_t)(n % 1034u);
    return period[idx];
}
