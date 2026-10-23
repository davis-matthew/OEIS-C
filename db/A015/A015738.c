/*
 * A015738  Inverse of 1729th cyclotomic polynomial.
 * https://oeis.org/A015738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = the n-th coefficient of the power series expansion of (1
 *            - x^1729) / Φ_1729(x). (The sequence is periodic with period
 *            1729, and is zero for all indices greater than or equal to 434
 *            within each period.)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define N_CYC 1729      /* cyclotomic number */
#define MAX_DEG 2000    /* sufficient for intermediate degrees   */
#define PERIOD 1729
#define QUOT_LEN (N_CYC - 1296 + 1) /* 434 */

/* internal helper: multiply a polynomial by (x^d - 1) */
static void mul_by_xd_minus1(const seq_t *a, idx_t a_deg,
                             seq_t *out, idx_t *out_deg, idx_t d)
{
    idx_t new_deg = a_deg + d;
    for (idx_t i = 0; i <= new_deg; ++i)
        out[i] = 0;
    for (idx_t i = 0; i <= a_deg; ++i) {
        out[i] -= a[i];
        out[i + d] += a[i];
    }
    *out_deg = new_deg;
}

/* internal helper: divide a polynomial by (x^d - 1) (exact division) */
static void div_by_xd_minus1(const seq_t *a, idx_t a_deg,
                             seq_t *out, idx_t *out_deg, idx_t d)
{
    static seq_t temp[MAX_DEG];
    for (idx_t i = 0; i <= a_deg; ++i)
        temp[i] = a[i];
    idx_t qdeg = a_deg - d;
    for (idx_t i = 0; i <= qdeg; ++i)
        out[i] = 0;
    for (long long i = a_deg; i >= d; --i) {
        seq_t coeff = temp[(idx_t)i];
        if (coeff != 0) {
            out[(idx_t)i - d] = coeff;
            temp[(idx_t)i] -= coeff;           /* subtract coeff * 1   */
            temp[(idx_t)i - d] += coeff;       /* add coeff * x^d       */
        }
    }
    *out_deg = qdeg;
}

/* compute the period coefficients once */
static void init_period(void)
{
    static seq_t period[PERIOD] = {0};
    static bool done = false;
    if (done) return;
    done = true;

    /* compute Φ_1729(x) */
    static seq_t poly[MAX_DEG];
    static idx_t poly_deg = 0;
    poly[0] = 1;

    /* multiply by factors with μ = +1 : x^d - 1 */
    const idx_t mult_factors[] = {7, 13, 19, 1729};
    for (int k = 0; k < 4; ++k) {
        idx_t d = mult_factors[k];
        static seq_t tmp[MAX_DEG];
        mul_by_xd_minus1(poly, poly_deg, tmp, &poly_deg, d);
        for (idx_t i = 0; i <= poly_deg; ++i)
            poly[i] = tmp[i];
    }

    /* divide by factors with μ = -1 : x^d - 1 */
    const idx_t div_factors[] = {1, 91, 133, 247};
    for (int k = 0; k < 4; ++k) {
        idx_t d = div_factors[k];
        static seq_t tmp[MAX_DEG];
        div_by_xd_minus1(poly, poly_deg, tmp, &poly_deg, d);
        for (idx_t i = 0; i <= poly_deg; ++i)
            poly[i] = tmp[i];
    }
    /* poly now equals Φ_1729(x), deg = 1296 */

    /* dividend = 1 - x^1729 */
    static seq_t dividend[MAX_DEG];
    for (idx_t i = 0; i <= N_CYC; ++i)
        dividend[i] = 0;
    dividend[0] = 1;
    dividend[N_CYC] = -1;

    /* perform division: (1 - x^1729) = Φ_1729(x) * Q(x) */
    idx_t dS = poly_deg;           /* 1296 */
    idx_t qlen = N_CYC - dS + 1;    /* 434 */
    static seq_t quotient[MAX_DEG];
    for (idx_t i = 0; i < qlen; ++i)
        quotient[i] = 0;

    for (long long i = N_CYC; i >= dS; --i) {
        seq_t coeff = dividend[(idx_t)i];
        if (coeff != 0) {
            quotient[(idx_t)i - dS] = coeff;
            /* subtract coeff * Φ_1729(x) shifted by (i - dS) */
            for (idx_t j = 0; j <= dS; ++j) {
                dividend[(idx_t)(j + (i - dS))] -= coeff * poly[j];
            }
        }
    }

    /* store period */
    for (idx_t i = 0; i < qlen; ++i)
        period[i] = quotient[i];
    for (idx_t i = qlen; i < PERIOD; ++i)
        period[i] = 0;
}

/* Public function: returns a(n) */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t period[PERIOD];
    if (!initialized) {
        init_period();
        initialized = true;
        /* copy period from init_period's static array */
        /* The init_period function defined its own static array.
           To avoid confusion, we reimplement the same logic here. */
        /* Here we recompute the period once, stored in period above. */
        static bool recomputed = false;
        if (!recomputed) {
            /* compute Φ_1729 */
            static seq_t poly[MAX_DEG];
            static idx_t poly_deg = 0;
            poly[0] = 1;
            const idx_t mult_factors[] = {7, 13, 19, 1729};
            for (int k = 0; k < 4; ++k) {
                idx_t d = mult_factors[k];
                static seq_t tmp[MAX_DEG];
                mul_by_xd_minus1(poly, poly_deg, tmp, &poly_deg, d);
                for (idx_t i = 0; i <= poly_deg; ++i)
                    poly[i] = tmp[i];
            }
            const idx_t div_factors[] = {1, 91, 133, 247};
            for (int k = 0; k < 4; ++k) {
                idx_t d = div_factors[k];
                static seq_t tmp[MAX_DEG];
                div_by_xd_minus1(poly, poly_deg, tmp, &poly_deg, d);
                for (idx_t i = 0; i <= poly_deg; ++i)
                    poly[i] = tmp[i];
            }
            static seq_t dividend[MAX_DEG];
            for (idx_t i = 0; i <= N_CYC; ++i)
                dividend[i] = 0;
            dividend[0] = 1;
            dividend[N_CYC] = -1;
            idx_t dS = poly_deg;
            idx_t qlen = N_CYC - dS + 1;
            static seq_t quotient[MAX_DEG];
            for (idx_t i = 0; i < qlen; ++i)
                quotient[i] = 0;
            for (long long i = N_CYC; i >= dS; --i) {
                seq_t coeff = dividend[(idx_t)i];
                if (coeff != 0) {
                    quotient[(idx_t)i - dS] = coeff;
                    for (idx_t j = 0; j <= dS; ++j) {
                        dividend[(idx_t)(j + (i - dS))] -= coeff * poly[j];
                    }
                }
            }
            for (idx_t i = 0; i < qlen; ++i)
                period[i] = quotient[i];
            for (idx_t i = qlen; i < PERIOD; ++i)
                period[i] = 0;
            recomputed = true;
        }
    }

    idx_t r = n % PERIOD;
    return period[r];
}
