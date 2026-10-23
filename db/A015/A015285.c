/*
 * A015285  Inverse of 1276th cyclotomic polynomial.
 * https://oeis.org/A015285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficients of the formal power series inverse of * the
 *            1276th cyclotomic polynomial Φ1276(x), i.e. * Φ1276(x) * a(x) =
 *            1 (as formal power series). The sequence is * periodic with
 *            period 1276 and a(0)=1. The recurrence derived from * Φ1276(x)
 *            yields a periodic sequence of length 1276
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define MAX_DIV 20
#define MAX_POLY 2000

/* Helper: polynomial multiplication                                          */
static void poly_mul(const seq_t *A, size_t dA,
                     const seq_t *B, size_t dB,
                     seq_t *R)
{
    size_t i, j;
    for (i = 0; i <= dA + dB; ++i) R[i] = 0;
    for (i = 0; i <= dA; ++i)
        for (j = 0; j <= dB; ++j)
            R[i + j] += A[i] * B[j];
}

/* Helper: division by a monic polynomial (leading coeff = 1)              */
static void poly_divmonic(const seq_t *N, size_t dN,
                          const seq_t *D, size_t dD,
                          seq_t *Q)
{
    static seq_t rem[MAX_POLY];
    size_t i, j;
    for (i = 0; i <= dN; ++i) rem[i] = N[i];
    for (i = 0; i <= dN - dD; ++i) Q[i] = 0;              /* Q size = dN-dD+1 */

    for (int k = (int)dN; k >= (int)dD; --k) {
        seq_t coeff = rem[k];
        if (coeff == 0) continue;                        /* no contribution */
        Q[k - dD] = coeff;                               /* leading coeff = 1 */
        for (j = 0; j <= dD; ++j)
            rem[k - dD + j] -= coeff * D[j];
    }
}

/* Compute all cyclotomic polynomials Φ_d for divisors of 1276 */
static void compute_phis(uint64_t *divs, size_t *cnt,
                         seq_t pcoef[MAX_DIV][MAX_POLY],
                         size_t pdeg[MAX_DIV])
{
    size_t i, j, k, ii;
    /* Collect divisors of 1276 */
    uint64_t n = 1276;
    for (uint64_t i0 = 1; i0 * i0 <= n; ++i0) {
        if (n % i0 == 0) {
            divs[(*cnt)++] = i0;
            if (i0 * i0 != n) divs[(*cnt)++] = n / i0;
        }
    }
    /* sort divisors */
    for (i = 0; i < *cnt; ++i)
        for (j = i + 1; j < *cnt; ++j)
            if (divs[i] > divs[j]) {
                uint64_t tmp = divs[i];
                divs[i] = divs[j];
                divs[j] = tmp;
            }

    static seq_t prod[MAX_POLY];
    static seq_t newprod[MAX_POLY];
    static seq_t N[MAX_POLY];
    static seq_t Q[MAX_POLY];

    for (i = 0; i < *cnt; ++i) {
        uint64_t d = divs[i];
        if (d == 1) {                     /* Φ_1(x) = x-1 */
            pdeg[i] = 1;
            pcoef[i][0] = -1;
            pcoef[i][1] =  1;
            continue;
        }

        /* product of Φ_e for e | d, e < d */
        prod[0] = 1;
        size_t degProd = 0;
        for (j = 0; j < i; ++j) {
            if (d % divs[j] == 0) {
                poly_mul(prod, degProd, pcoef[j], pdeg[j], newprod);
                size_t newDeg = degProd + pdeg[j];
                for (k = 0; k <= newDeg; ++k) prod[k] = newprod[k];
                degProd = newDeg;
            }
        }

        /* N = x^d - 1 */
        for (k = 0; k <= d; ++k) N[k] = 0;
        N[0] = -1;
        N[d] = 1;

        poly_divmonic(N, d, prod, degProd, Q);
        size_t degPhi = d - degProd;          /* degree of Φ_d */
        pdeg[i] = degPhi;
        for (k = 0; k <= degPhi; ++k) pcoef[i][k] = Q[k];
    }
}

/* Main function computing the sequence value a(n) */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t period[1276];

    if (!initialized) {
        uint64_t divs[MAX_DIV];
        size_t div_cnt = 0;
        seq_t pcoef[MAX_DIV][MAX_POLY];
        size_t pdeg[MAX_DIV];

        compute_phis(divs, &div_cnt, pcoef, pdeg);

        /* locate Φ_1276 among the computed polynomials */
        size_t idx1276 = 0;
        while (divs[idx1276] != 1276) ++idx1276;

        /* compute the periodic sequence using recurrence from Φ_1276 */
        period[0] = 1;
        size_t d = pdeg[idx1276];
        for (size_t k = 1; k < 1276; ++k) {
            seq_t sum = 0;
            size_t lim = k < d ? k : d;
            for (size_t i = 1; i <= lim; ++i)
                sum += pcoef[idx1276][i] * period[k - i];
            period[k] = -sum;
        }

        initialized = true;
    }

    idx_t idx = n % 1276;
    return period[(size_t)idx];
}
