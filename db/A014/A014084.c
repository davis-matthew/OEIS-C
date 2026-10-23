/*
 * A014084  Inverse of 75th cyclotomic polynomial.
 * https://oeis.org/A014084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the power series expansion of
 *            1 / Φ₇₅(x). The cyclotomic polynomial Φ₇₅(x) has degree φ(75)=40
 *            and its coefficients are integers in {−1,0,1}. The reciprocal
 *            series satisfies a(0) = 1; a(n) = −∑_{k=1}^{40} coeff[k] ·
 *            a(n−k) for n ≥ 1, where coeff[k] are the coeffi...
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* theorem: DEGR = φ(75) = 40, PERIOD = 75 */
#define DEGR   40
#define PERIOD 75

/* Coefficients of Φ₇₅(x).  coeff[0] = 1. */
static int coeff[DEGR + 1];
static bool coeff_inited = false;

/* Compute the coefficients of Φ₇₅(x) by long division:
   (x^80 - x^75 - x^5 + 1) / (x^40 - x^25 - x^15 + 1). */
static void init_coeff(void)
{
    if (coeff_inited)
        return;

    /* numerator and denominator polynomials */
    int numer[81] = {0};
    int denom[41] = {0};

    numer[0]  =  1;   /* +1          */
    numer[5]  = -1;   /* -x^5        */
    numer[75] = -1;   /* -x^75       */
    numer[80] =  1;   /* +x^80       */

    denom[0]  = 1;    /* +1          */
    denom[15] = -1;   /* -x^15       */
    denom[25] = -1;   /* -x^25       */
    denom[40] = 1;    /* +x^40       */

    int quotient[41] = {0};

    /* long division: from high degree down to 40 */
    for (int i = 80; i >= 40; --i) {
        int c = numer[i];
        if (c == 0)
            continue;
        int shift = i - 40;              /* current degree of quotient term */
        quotient[shift] = c;
        for (int j = 0; j <= 40; ++j) {
            numer[shift + j] -= c * denom[j];
        }
    }

    /* copy quotient as coeff[0..DEGR] */
    for (int k = 0; k <= DEGR; ++k) {
        coeff[k] = quotient[k];
    }

    coeff_inited = true;
}

/* Periodic sequence a(n) cache */
static seq_t *period = NULL;
static bool period_inited = false;

/* Build the first PERIOD terms of the sequence using the recurrence. */
static void init_period(void)
{
    if (period_inited)
        return;

    init_coeff();

    period = (seq_t*)malloc(PERIOD * sizeof(seq_t));
    if (!period)
        abort();

    period[0] = 1; /* a(0) = 1 */

    for (int n = 1; n < PERIOD; ++n) {
        seq_t sum = 0;
        int kmax = (n < DEGR) ? n : DEGR;      /* number of terms in sum */
        for (int k = 1; k <= kmax; ++k) {
            sum -= (seq_t)coeff[k] * period[n - k];
        }
        period[n] = sum;
    }

    period_inited = true;
}

/* Return the n‑th term of OEIS A014084. */
seq_t a(idx_t n)
{
    if (!period_inited)
        init_period();

    return period[(size_t)(n % PERIOD)];
}
