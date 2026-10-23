/*
 * A014451  Inverse of 442nd cyclotomic polynomial.
 * https://oeis.org/A014451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of 1/Φ_442(x); period = 442
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Helper: multiply polynomials A*B.
   A has degree degA, B has degree degB.
   Result is stored in *C with degree *degC = degA+degB. */
static void poly_mul(const seq_t *A, size_t degA,
                     const seq_t *B, size_t degB,
                     seq_t **C, size_t *degC) {
    size_t deg = degA + degB;
    seq_t *R = (seq_t *)malloc((deg + 1) * sizeof(seq_t));
    memset(R, 0, (deg + 1) * sizeof(seq_t));
    for (size_t i = 0; i <= degA; ++i)
        for (size_t j = 0; j <= degB; ++j)
            R[i + j] += A[i] * B[j];
    *C = R;
    *degC = deg;
}

/* Helper: divide monic polynomial N by D.
   DegN >= DegD.  Result Q has degree degN-degD.  D is monic (leading coeff = 1). */
static void poly_divide(seq_t *N, size_t degN,
                        const seq_t *D, size_t degD,
                        seq_t **Q, size_t *degQ) {
    seq_t *R = (seq_t *)malloc((degN + 1) * sizeof(seq_t));
    memcpy(R, N, (degN + 1) * sizeof(seq_t));
    size_t qdeg = degN - degD;
    seq_t *quot = (seq_t *)malloc((qdeg + 1) * sizeof(seq_t));
    memset(quot, 0, (qdeg + 1) * sizeof(seq_t));

    for (int i = (int)degN; i >= (int)degD; --i) {
        seq_t coeff = R[i];          /* D[degD] == 1 */
        int shift = i - (int)degD;
        quot[shift] = coeff;
        for (size_t j = 0; j <= degD; ++j)
            R[shift + j] -= coeff * D[j];
    }
    /* remainder should be zero; ignore it */
    free(R);
    *Q = quot;
    *degQ = qdeg;
}

/* Compute polynomial Φ_442(x).  Result stored in *phi and its degree *degPhi. */
static void compute_phi_442(seq_t **phi, size_t *degPhi) {
    /* Divisors of 442 and their Möbius values */
    static const idx_t divisors[] = {1,2,13,17,26,34,221,442};
    static const int mu[] = {1,-1,-1,-1,1,1,1,-1};
    const size_t nd = sizeof(divisors)/sizeof(divisors[0]);

    /* Numerator polynomial (product of (x^k-1) for mu==1) */
    seq_t *num = (seq_t *)malloc(sizeof(seq_t));  /* start with polynomial 1 */
    num[0] = 1;
    size_t degNum = 0;

    /* Denominator polynomial (product of (x^k-1) for mu==-1) */
    seq_t *den = (seq_t *)malloc(sizeof(seq_t));
    den[0] = 1;
    size_t degDen = 0;

    for (size_t idx = 0; idx < nd; ++idx) {
        idx_t d = divisors[idx];
        int mu_val = mu[idx];
        size_t k = 442 / d;          /* power in (x^k - 1) */

        /* Build factor (x^k - 1): coefficients [-1, 0, ..., 0, 1] */
        seq_t *fac = (seq_t *)calloc(k + 1, sizeof(seq_t));
        fac[0] = -1;
        fac[k] = 1;

        if (mu_val == 1) {
            seq_t *tmp;
            size_t tmpDeg;
            poly_mul(num, degNum, fac, k, &tmp, &tmpDeg);
            free(num);
            num = tmp;
            degNum = tmpDeg;
        } else {
            seq_t *tmp;
            size_t tmpDeg;
            poly_mul(den, degDen, fac, k, &tmp, &tmpDeg);
            free(den);
            den = tmp;
            degDen = tmpDeg;
        }
        free(fac);
    }

    /* Compute Φ_442 = num / den  (den is monic) */
    seq_t *phiPoly;
    size_t phiDeg;
    poly_divide(num, degNum, den, degDen, &phiPoly, &phiDeg);

    free(num);
    free(den);

    *phi = phiPoly;
    *degPhi = phiDeg;
}

/* Public function returning a(n) for OEIS A014451 */
seq_t a(idx_t n) {
    static bool init = false;
    static seq_t period[442];

    if (!init) {
        /* compute Φ_442 */
        seq_t *phi;
        size_t degPhi;
        compute_phi_442(&phi, &degPhi);
        /* Compute sequence up to period length 442 */
        period[0] = 1;
        for (size_t m = 1; m < 442; ++m) {
            seq_t sum = 0;
            size_t maxk = m < degPhi ? m : degPhi;
            for (size_t k = 1; k <= maxk; ++k)
                sum += phi[k] * period[m - k];
            period[m] = -sum;            /* because Φ polynomial has constant term 1 */
        }
        free(phi);
        init = true;
    }
    return period[n % 442];
}
