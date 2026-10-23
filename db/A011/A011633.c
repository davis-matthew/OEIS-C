/*
 * A011633  30th cyclotomic polynomial.
 * https://oeis.org/A011633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of xⁿ in the 30th cyclotomic polynomial
 *            Φ₃₀(x)
 * Range:     i8 n <= 8, i16 n <= 8, i32 n <= 8, i64 n <= 8, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Polynomial degree is 8; a(n)=0 for n>8.                                      */

#include <stdbool.h>
#include <stddef.h>

/* multiply polynomials A (degA) and B (degB); result in R, degR returned.   */
static void poly_mul(const seq_t *A, idx_t degA,
                     const seq_t *B, idx_t degB,
                     seq_t *R, idx_t *degR)
{
    /* initialise result */
    for (idx_t i = 0; i <= degA + degB; ++i) R[i] = 0;

    for (idx_t i = 0; i <= degA; ++i)
        for (idx_t j = 0; j <= degB; ++j)
            R[i + j] += A[i] * B[j];

    *degR = degA + degB;
}

/* exact division of P (degP) by Q (degQ). Q has leading coefficient 1.   */
/* Quotient stored in R, degR returned.                                   */
static void poly_div_exact(const seq_t *P, idx_t degP,
                           const seq_t *Q, idx_t degQ,
                           seq_t *R, idx_t *degR)
{
    /* work array = dividend copy */
    seq_t work[64];
    for (idx_t i = 0; i <= degP; ++i) work[i] = P[i];
    for (idx_t i = degP + 1; i <= 63; ++i) work[i] = 0;

    /* initialise quotient */
    for (idx_t i = 0; i <= 63; ++i) R[i] = 0;
    *degR = degP - degQ;

    for (idx_t i = degP; i >= degQ; --i) {
        seq_t coeff = work[i];          /* leading coefficient */
        if (coeff != 0) {
            R[i - degQ] = coeff;
            /* subtract coeff * Q shifted by (i - degQ) */
            idx_t shift = i - degQ;
            for (idx_t j = 0; j <= degQ; ++j)
                work[shift + j] -= coeff * Q[j];
        }
    }
}

/* compute the coefficients of Φ₃₀(x).  result stored in coeff[0..8].        */
static void compute_phi30(seq_t *coeff)
{
    /* divisors of 30 */
    const idx_t d[8] = {1, 2, 3, 5, 6, 10, 15, 30};
    /* exponent μ(30/d) for each divisor */
    const int exp[8] = {-1, 1, 1, 1, -1, -1, -1, 1};

    seq_t num[64] = {0};
    seq_t den[64] = {0};
    idx_t degNum = 0, degDen = 0;
    num[0] = 1;           /* start with polynomial 1 */
    den[0] = 1;

    seq_t factor[64];
    for (idx_t k = 0; k < 8; ++k) {
        /* factor = (x^d[k] - 1) */
        for (idx_t i = 0; i <= 63; ++i) factor[i] = 0;
        factor[0] = -1;
        factor[d[k]] = 1;
        idx_t degF = d[k];

        if (exp[k] == 1) {
            seq_t res[64]; idx_t degR;
            poly_mul(num, degNum, factor, degF, res, &degR);
            for (idx_t i = 0; i <= degR; ++i) num[i] = res[i];
            degNum = degR;
        } else if (exp[k] == -1) {
            seq_t res[64]; idx_t degR;
            poly_mul(den, degDen, factor, degF, res, &degR);
            for (idx_t i = 0; i <= degR; ++i) den[i] = res[i];
            degDen = degR;
        }
    }

    /* divide numerator by denominator */
    seq_t quo[64]; idx_t degQ;
    poly_div_exact(num, degNum, den, degDen, quo, &degQ);

    /* copy result to coeff[0..8] (degree 8) */
    for (idx_t i = 0; i <= 8; ++i)
        coeff[i] = quo[i];
}

/* public interface required by the test harness                      */
seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t coeff[9];

    if (!init) {
        compute_phi30(coeff);
        init = true;
    }

    if (n > 8) return 0;
    return coeff[n];
}
