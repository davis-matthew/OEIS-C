/*
 * A015186  Inverse of 1177th cyclotomic polynomial.
 * https://oeis.org/A015186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1/Φ_1177(x))_n (the coefficient of xⁿ in the power
 *            series expansion of the reciprocal of the 1177th cyclotomic
 *            polynomial Φ_1177(x). The sequence is periodic with period 1177.
 *            a(0) = 1. For n ≥ 1: a(n) = –∑_{k=1}^{min(n,φ(1177))} φ_k ·
 *            a(n–k), where φ_k are the coefficients of Φ_1177(x)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq.h"  /* alloc_seq(size_t) returns zeroed seq_t* */

/* compute the coefficients of Φ_1177(x);
   φ[0] will be set to 1 and φ[1..1060] to the remaining coefficients. */
static void compute_coefficients(seq_t *phi)
{
    /* 1177 = 11 × 107, so φ(1177) = (11–1)(107–1) = 1060. */
    const idx_t degPhi = 1060;
    const idx_t degP   = 1178;   /* degree of (x-1)(x^1177-1) = 1178 */
    const idx_t degB   = 118;    /* degree of (x^11-1)(x^107-1) = 118 */

    /* dividend: P(x) = (x-1)(x^n-1) = x^1178 – x^1177 – x + 1 */
    seq_t *P = alloc_seq(degP + 1);
    P[0] = 1;          /* constant term */
    P[1] = -1;         /* coefficient of x */
    P[1177] = -1;      /* coefficient of x^1177 */
    P[1178] = 1;       /* coefficient of x^1178 */

    /* divisor: B(x) = (x^11-1)(x^107-1) = x^118 – x^107 – x^11 + 1 */
    seq_t *B = alloc_seq(degB + 1);
    B[0] = 1;          /* constant term */
    B[11] = -1;        /* coefficient of x^11 */
    B[107] = -1;       /* coefficient of x^107 */
    B[118] = 1;        /* coefficient of x^118 */

    /* Perform polynomial long division P(x) / B(x) to obtain Φ_1177(x). */
    for (idx_t i = degP; i >= degB; --i)
    {
        seq_t coeff = P[i];            /* the leading coefficient */
        if (coeff != 0)
        {
            idx_t shift = i - degB;    /* how many places B is shifted */
            phi[shift] = coeff;        /* store coefficient of Φ */
            /* subtract coeff * B(x) * x^shift from P */
            for (idx_t j = 0; j <= degB; ++j)
                P[shift + j] -= coeff * B[j];
        }
    }

    /* cleanup */
    free(P);
    free(B);
}

/* compute all 1177 terms of the periodic sequence once
   and cache them for subsequent calls. */
static seq_t *compute_periodic(void)
{
    const idx_t period = 1177;
    seq_t *periodic = alloc_seq(period);

    /* Get coefficients of Φ_1177(x). */
    const idx_t degPhi = 1060;
    seq_t *phi = alloc_seq(degPhi + 1);
    compute_coefficients(phi);

    /* a(0) = 1. */
    periodic[0] = 1;

    /* Use the recurrence derived from Φ_1177(x). */
    for (idx_t n = 1; n < period; ++n)
    {
        seq_t sum = 0;
        idx_t maxk = (n < degPhi) ? n : degPhi;
        for (idx_t k = 1; k <= maxk; ++k)
            sum += phi[k] * periodic[n - k];
        periodic[n] = -sum;
    }

    free(phi);
    return periodic;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static seq_t *periodic = NULL;
    if (periodic == NULL)
        periodic = compute_periodic();

    return periodic[n % 1177];
}
