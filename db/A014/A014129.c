/*
 * A014129  Inverse of 120th cyclotomic polynomial.
 * https://oeis.org/A014129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of xⁿ in the power series expansion of
 *            1/Φ₁₂₀(x). With Φ₁₂₀(x)=∑_{i=0}^{32} c_i x^i (c₀=c₃₂=1) we use
 *            the recurrence a(0)=1, a(n) = -∑_{i=1}^{min(32,n)} c_i · a(n‑i)
 *            for n≥1. The sequence is periodic with period 120
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* forward declarations of helper routines, private to this file */
static void compute_coeffs(void);

/* globals holding the period and its cyclotomic coefficients */
static seq_t g_cycle[120];
static seq_t g_c_phi[33];
static bool g_inited = false;

/* ------------------------------------------------------------------ */
/*  Helper: multiply two polynomials a (degree da) and b (degree db).
        Result is written into res and deg_res is set to degree of the product.
*/
static void poly_mul(const seq_t *a, int da, const seq_t *b, int db,
                     seq_t *res, int *deg_res)
{
    int i, j;
    for (i = 0; i <= da + db; ++i)
        res[i] = 0;
    for (i = 0; i <= da; ++i)
        for (j = 0; j <= db; ++j)
            res[i + j] += a[i] * b[j];
    *deg_res = da + db;
}

/* ------------------------------------------------------------------ */
/*  Helper: divide a monic polynomial num (degree dnum) by a monic
        polynomial den (degree dden) giving quotient quot of degree
        dnum - dden. Division is exact. */
static void poly_div_monic(const seq_t *num, int dnum,
                           const seq_t *den, int dden,
                           seq_t *quot)
{
    int i, j;
    seq_t rem[200];
    int drem = dnum;

    /* copy numerator into temporary remainder array */
    for (i = 0; i <= dnum; ++i)
        rem[i] = num[i];

    int qdeg = dnum - dden;
    for (i = dnum; i >= dden; --i) {
        seq_t coeff = rem[i];               /* den[dden] == 1 */
        quot[i - dden] = coeff;
        /* subtract coeff*den shifted by (i - dden) from remainder */
        for (j = 0; j <= dden; ++j)
            rem[i - dden + j] -= coeff * den[j];
    }
    /* quotient coefficients for degrees < qdeg are already zero */
}

/* ------------------------------------------------------------------ */
/*  Compute the coefficients of Φ120(x) and the 120-period of its
    reciprocal. Called only once. */
static void compute_coeffs(void)
{
    /* ------------------------------------------------------------------
       Step 1: build numerator = Π (x^k - 1) for ks = {120,20,12,8}
       Step 2: build denominator = Π (x^k - 1) for ks = {60,40,24,4}
       Step 3: divide to get Φ120(x)
    ------------------------------------------------------------------ */

    const int num_ks[]   = {120, 20, 12,  8};
    const int den_ks[]   = { 60, 40, 24,  4};
    const int NUM_KS = sizeof(num_ks) / sizeof(num_ks[0]);
    const int DEN_KS = sizeof(den_ks) / sizeof(den_ks[0]);

    seq_t tmp[200];
    seq_t res[200];
    int dtmp = 0, dres = 0;
    int i, j, k;

    /* -------- Numerator -------- */
    seq_t num[200] = {0};
    int dnum = 0;
    num[0] = 1;                /* start with polynomial 1 */

    for (k = 0; k < NUM_KS; ++k) {
        int kdeg = num_ks[k];
        for (i = 0; i <= kdeg; ++i)
            tmp[i] = 0;
        tmp[0]    = -1;
        tmp[kdeg] =  1;
        dtmp = kdeg;
        poly_mul(num, dnum, tmp, dtmp, res, &dres);
        for (i = 0; i <= dres; ++i)
            num[i] = res[i];
        dnum = dres;
    }

    /* -------- Denominator -------- */
    seq_t den[200] = {0};
    int dden = 0;
    den[0] = 1;                /* start with polynomial 1 */

    for (k = 0; k < DEN_KS; ++k) {
        int kdeg = den_ks[k];
        for (i = 0; i <= kdeg; ++i)
            tmp[i] = 0;
        tmp[0]    = -1;
        tmp[kdeg] =  1;
        dtmp = kdeg;
        poly_mul(den, dden, tmp, dtmp, res, &dres);
        for (i = 0; i <= dres; ++i)
            den[i] = res[i];
        dden = dres;
    }

    /* -------- Division: Φ120 = Numerator / Denominator -------- */
    seq_t quot[200] = {0};
    poly_div_monic(num, dnum, den, dden, quot);

    /* Store the coefficients c0..c32 (c0 = 1, c32 = 1) */
    for (i = 0; i <= 32; ++i)
        g_c_phi[i] = quot[i];

    /* -------- Compute the 120-term period via recurrence -------- */
    seq_t a_cycle[120] = {0};
    a_cycle[0] = 1;
    for (i = 1; i < 120; ++i) {
        seq_t sum = 0;
        int lim = (i < 32) ? i : 32;
        for (j = 1; j <= lim; ++j)
            sum += g_c_phi[j] * a_cycle[i - j];
        a_cycle[i] = -sum;
    }

    /* copy to global array */
    for (i = 0; i < 120; ++i)
        g_cycle[i] = a_cycle[i];
}

/* ------------------------------------------------------------------ */
/*  Public sequence function required by the framework.               */
seq_t a(idx_t n)
{
    if (!g_inited) {
        compute_coeffs();
        g_inited = true;
    }
    return g_cycle[(size_t)(n % 120)];
}
