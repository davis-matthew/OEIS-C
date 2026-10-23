/*
 * A015254  Inverse of 1245th cyclotomic polynomial.
 * https://oeis.org/A015254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; for n≥1: a(n) = -∑_{k=1}^{min(n,656)} u_k * a(n‑k),
 *            where u_k are the coefficients of the 1245th cyclotomic
 *            polynomial Φ_1245(x). The sequence is periodic with period 1245
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Polynomial multiplication: R = A * B, degR = degA + degB */
static void poly_mult(const seq_t *A, size_t degA,
                      const seq_t *B, size_t degB,
                      seq_t *R, size_t degR)
{
    size_t i, j;
    for (i = 0; i <= degR; ++i) R[i] = 0;
    for (i = 0; i <= degA; ++i)
        for (j = 0; j <= degB; ++j)
            R[i + j] += A[i] * B[j];
}

/* Polynomial division: Q = A / B  (|A|≥|B|, B monic with leading coeff 1) */
static void poly_div(const seq_t *A, size_t degA,
                     const seq_t *B, size_t degB,
                     seq_t *Q, size_t degQ)
{
    static seq_t R[2000];
    size_t i;
    for (i = 0; i <= degA; ++i) R[i] = A[i];
    for (i = 0; i <= degQ; ++i) Q[i] = 0;

    for (int ii = (int)degA; ii >= (int)degB; --ii) {
        seq_t coeff = R[ii];
        size_t qpos = ii - degB;
        Q[qpos] = coeff;
        if (coeff != 0) {
            for (i = 0; i <= degB; ++i)
                R[qpos + i] -= coeff * B[i];
        }
    }
    /* R now holds the remainder, which should be zero by construction */
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t period[1245];
    static seq_t coeff[657];   /* Φ₁₂₄₅ has degree 656 */
    static seq_t poly[8][2000];
    static size_t deg[8];

    if (!initialized) {
        /* Divisors of 1245 in ascending order */
        const idx_t divs[8] = {1, 3, 5, 15, 83, 249, 415, 1245};

        /* Compute each Φ_d sequentially */
        for (size_t i = 0; i < 8; ++i) {
            const idx_t d = divs[i];

            if (d == 1) {
                /* Φ₁(x) = x – 1 */
                for (size_t k = 0; k < 2000; ++k) poly[i][k] = 0;
                poly[i][0] = -1;
                poly[i][1] = 1;
                deg[i] = 1;
                continue;
            }

            /* Build denominator = product of Φ_e for e | d, e < d */
            seq_t denom[2000];
            size_t degDen = 0;
            for (size_t k = 0; k < 2000; ++k) denom[k] = 0;
            denom[0] = 1;           /* start with constant 1 */
            for (size_t j = 0; j < i; ++j) {
                if (d % divs[j] == 0) {
                    seq_t tmp[2000];
                    size_t newDeg = degDen + deg[j];
                    poly_mult(denom, degDen, poly[j], deg[j], tmp, newDeg);
                    degDen = newDeg;
                    for (size_t t = 0; t <= degDen; ++t) denom[t] = tmp[t];
                }
            }

            /* Numerator: x^d – 1 */
            seq_t num[2000];
            size_t degNum = (size_t)d;
            for (size_t k = 0; k <= degNum; ++k) num[k] = 0;
            num[0] = -1;
            num[degNum] = 1;

            /* Divide numerator by denominator to obtain Φ_d */
            seq_t res[2000];
            size_t degRes = degNum - degDen;
            poly_div(num, degNum, denom, degDen, res, degRes);

            deg[i] = degRes;
            for (size_t t = 0; t <= degRes; ++t) poly[i][t] = res[t];
        }

        /* Φ₁₂₄₅ is the last polynomial */
        memcpy(coeff, poly[7], (deg[7] + 1) * sizeof(seq_t));

        /* Compute the period coefficients a(0)..a(1244) */
        period[0] = 1;
        for (size_t i = 1; i < 1245; ++i) {
            seq_t sum = 0;
            size_t maxk = (i < deg[7]) ? i : deg[7];
            for (size_t k = 1; k <= maxk; ++k)
                sum += coeff[k] * period[i - k];
            period[i] = -sum;
        }

        initialized = true;
    }

    return period[(size_t)(n % 1245)];
}
