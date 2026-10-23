/*
 * A015569  Inverse of 1560th cyclotomic polynomial.
 * https://oeis.org/A015569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in the power series inverse of
 *            Φ_1560(x) modulo x^1560-1. The sequence is periodic with period
 *            1560
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* The types seq_t and idx_t are supplied by the build environment. */
typedef seq_t seq_t;
typedef idx_t idx_t;

/* ------------------------------------------------------------------ */
/* Helper: division of a monic polynomial divisor from a dividend    */
/* ------------------------------------------------------------------ */
static seq_t *poly_div_monic(const seq_t *dividend, size_t m,
                             const seq_t *divisor, size_t k)
{
    /* divisor is monic, m >= k.  The quotient has degree m-k. */
    size_t qlen = m - k + 1;
    seq_t *quot = (seq_t *)calloc(qlen, sizeof(seq_t));      /* zeros */
    seq_t *rem  = (seq_t *)malloc((m + 1) * sizeof(seq_t));
    memcpy(rem, dividend, (m + 1) * sizeof(seq_t));

    for (long long i = (long long)m; i >= (long long)k; --i) {
        seq_t coef = rem[(size_t)i];
        if (coef == 0)
            continue;
        size_t qidx = (size_t)(i - k);
        quot[qidx] = coef;
        /* subtract coef * divisor shifted to match this term */
        for (size_t j = 0; j <= k; ++j) {
            rem[j + qidx] -= coef * divisor[j];
        }
        /* rem[i] should now be zero (leading coefficient subtracted) */
    }

    free(rem);
    return quot;
}

/* ------------------------------------------------------------------ */
/*  Compute all cyclotomic polynomials Φ_d for d | 1560 and store     */
/*  Φ_1560 in the global array phi_coeff with degree phi_deg.        */
/* ------------------------------------------------------------------ */
static seq_t phi_coeff[385];        /* degree φ(1560) = 384   */
static size_t phi_deg;              /* |Φ_1560|               */
static int phi_inited = 0;

/* ------------------------------------------------------------------ */
static void init_phi(void)
{
    const idx_t N = 1560;

    /* List divisors of N */
    idx_t divs[64];
    size_t nd = 0;
    for (idx_t i = 1; i <= N; ++i)
        if (N % i == 0)
            divs[nd++] = i;

    /* sort ascending (simple insertion) */
    for (size_t i = 1; i < nd; ++i) {
        for (size_t j = i; j > 0; --j)
            if (divs[j] < divs[j - 1]) {
                idx_t t = divs[j];
                divs[j] = divs[j - 1];
                divs[j - 1] = t;
            }
    }

    /* per-divisor polynomial */
    struct { seq_t *coeff; size_t deg; } polys[64];
    for (size_t k = 0; k < nd; ++k)
        polys[k].coeff = NULL;

    /* Build Φ_d for each divisor d */
    for (size_t k = 0; k < nd; ++k) {
        idx_t d = divs[k];
        size_t deg = (size_t)d;

        /* start with x^d - 1 */
        seq_t *num = (seq_t *)calloc(deg + 1, sizeof(seq_t));
        num[0] = (seq_t)-1;
        num[deg] = (seq_t)1;

        /* divide by all smaller Φ_e with e | d */
        for (size_t l = 0; l < k; ++l) {
            if (d % divs[l] != 0)
                continue;
            seq_t *q = poly_div_monic(num, deg,
                                      polys[l].coeff, polys[l].deg);
            free(num);
            num = q;
            deg -= polys[l].deg;
        }

        polys[k].coeff = num;
        polys[k].deg   = deg;
    }

    /* φ_1560 is the last divisor */
    phi_deg = polys[nd - 1].deg;
    memcpy(phi_coeff, polys[nd - 1].coeff, (phi_deg + 1) * sizeof(seq_t));

    /* free temporary polys (except the last, already copied) */
    for (size_t k = 0; k < nd - 1; ++k)
        free(polys[k].coeff);
    /* but we also need to free the last polynomial after copy */
    free(polys[nd - 1].coeff);

    phi_inited = 1;
}

/* ------------------------------------------------------------------ */
/*  Compute the periodic sequence a(n) from the coefficients of Φ_1560  */
/* ------------------------------------------------------------------ */
static seq_t a_seq[1560];
static int a_inited = 0;

static void init_sequence(void)
{
    if (!phi_inited)
        init_phi();

    const size_t PERIOD = 1560;
    a_seq[0] = (seq_t)1;

    for (size_t n = 1; n < PERIOD; ++n) {
        seq_t sum = (seq_t)0;
        size_t lim = (n < phi_deg) ? n : phi_deg;
        for (size_t i = 1; i <= lim; ++i) {
            sum += phi_coeff[i] * a_seq[n - i];
        }
        a_seq[n] = -sum;
    }

    a_inited = 1;
}

/* ------------------------------------------------------------------ */
/*  Public function required by the framework.                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!a_inited)
        init_sequence();

    const size_t PERIOD = 1560;
    return a_seq[(size_t)(n % PERIOD)];
}
