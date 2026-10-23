/*
 * A016024  Inverse of 2015th cyclotomic polynomial.
 * https://oeis.org/A016024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = coefficient of x^n in the power series expansion of the
 *            reciprocal of the 2015th cyclotomic polynomial Φ₍₂₀₁₅₎(x). The
 *            sequence is periodic with period 2015
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define NUM_DEG_MAX 2064   /* degree of numerator polynomial */
#define DEN_DEG_MAX 624    /* degree of denominator polynomial */
#define PHI_DEG      1440  /* degree of Φ₍₂₀₁₅₎(x) */
#define PERIOD       2015

static seq_t period[PERIOD];
static bool  initialized = false;

static void compute_period(void)
{
    /* Temporary arrays of compile-constant sizes */
    static seq_t num[NUM_DEG_MAX + 1];
    static seq_t den[DEN_DEG_MAX + 1];
    static seq_t phi[PHI_DEG + 1];
    static seq_t tmp[NUM_DEG_MAX + 1];

    /* --------------------------------------------------------------------
       Build numerator:  (x^5−1)(x^13−1)(x^31−1)(x^2015−1)
       -------------------------------------------------------------------- */
    int deg = 0;
    for (int i = 0; i <= NUM_DEG_MAX; ++i) num[i] = 0;
    num[0] = 1;
    const int num_factors[4] = {5, 13, 31, 2015};
    for (int fi = 0; fi < 4; ++fi) {
        int d = num_factors[fi];
        int new_deg = deg + d;
        for (int i = 0; i <= new_deg; ++i) tmp[i] = 0;
        for (int i = 0; i <= deg; ++i) {
            /* (x^d − 1) = −1 + x^d */
            tmp[i]     -= num[i];
            tmp[i + d] += num[i];
        }
        for (int i = 0; i <= new_deg; ++i) num[i] = tmp[i];
        deg = new_deg;
    }

    /* --------------------------------------------------------------------
       Build denominator:  (x−1)(x^65−1)(x^155−1)(x^403−1)
       -------------------------------------------------------------------- */
    deg = 0;
    for (int i = 0; i <= DEN_DEG_MAX; ++i) den[i] = 0;
    den[0] = 1;
    const int den_factors[4] = {1, 65, 155, 403};
    for (int fi = 0; fi < 4; ++fi) {
        int d = den_factors[fi];
        int new_deg = deg + d;
        for (int i = 0; i <= new_deg; ++i) tmp[i] = 0;
        for (int i = 0; i <= deg; ++i) {
            tmp[i]     -= den[i];
            tmp[i + d] += den[i];
        }
        for (int i = 0; i <= new_deg; ++i) den[i] = tmp[i];
        deg = new_deg;
    }

    /* --------------------------------------------------------------------
       Long division num / den  →  Φ₍₂₀₁₅₎(x)
       -------------------------------------------------------------------- */
    /* use 'num' in place for the work */
    for (int k = NUM_DEG_MAX; k >= DEN_DEG_MAX; --k) {
        seq_t coeff = num[k];
        if (coeff == 0) continue;
        int q = k - DEN_DEG_MAX;
        phi[q] = coeff;
        for (int j = 0; j <= DEN_DEG_MAX; ++j) {
            num[q + j] -= coeff * den[j];
        }
    }

    /* --------------------------------------------------------------------
       Compute inverse series a(n) with recurrence:
           a(0) = 1
           a(n) = −∑_{i=1}^{PHI_DEG} φ_i · a(n−i),   n ≥ 1
       -------------------------------------------------------------------- */
    static seq_t series[PERIOD];
    for (int i = 0; i < PERIOD; ++i) series[i] = 0;
    series[0] = 1;
    for (int n = 1; n < PERIOD; ++n) {
        seq_t sum = 0;
        int max_i = (n < PHI_DEG) ? n : PHI_DEG;
        for (int i = 1; i <= max_i; ++i) {
            sum += phi[i] * series[n - i];
        }
        series[n] = -sum;
    }

    /* Copy the period into the table */
    for (int i = 0; i < PERIOD; ++i) period[i] = series[i];
}

/* External linkage, as required */
seq_t a(idx_t n)
{
    if (!initialized) {
        compute_period();
        initialized = true;
    }
    return period[n % PERIOD];
}
