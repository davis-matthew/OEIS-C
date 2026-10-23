/*
 * A016004  Inverse of 1995th cyclotomic polynomial.
 * https://oeis.org/A016004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = -∑_{k=1}^{deg} coeff[k] * a(n-k), where coeff[k] are the
 *            coefficients of Φ₁₉₉₅(x) (excluding the constant term). The
 *            sequence is periodic with period 1995
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* a helper structure for a polynomial */
typedef struct {
    size_t len;   /* number of coefficients (degree+1) */
    seq_t *coeff; /* coefficients, coeff[0] is the constant term */
} Poly;

/* multiply two polynomials */
static Poly poly_mul(const Poly *a, const Poly *b)
{
    Poly r;
    r.len = a->len + b->len - 1;
    r.coeff = (seq_t*)calloc(r.len, sizeof(seq_t));
    for (size_t i = 0; i < a->len; ++i) {
        for (size_t j = 0; j < b->len; ++j) {
            r.coeff[i + j] += a->coeff[i] * b->coeff[j];
        }
    }
    return r;
}

/* exact polynomial division: numerator / divisor (divisor is monic) */
static Poly poly_div(const seq_t *num, size_t num_len, const Poly *den)
{
    size_t degN = num_len - 1;
    size_t degD = den->len - 1;
    size_t qlen = degN - degD + 1;
    seq_t *quot = (seq_t*)calloc(qlen, sizeof(seq_t));
    seq_t *rem  = (seq_t*)calloc(num_len, sizeof(seq_t));
    for (size_t i = 0; i < num_len; ++i) rem[i] = num[i];

    for (int i = (int)(degN - degD); i >= 0; --i) {
        seq_t coeff = rem[degD + i];
        if (coeff != 0) {
            quot[i] = coeff;
            for (size_t j = 0; j < den->len; ++j) {
                rem[j + i] -= coeff * den->coeff[j];
            }
        }
    }
    free(rem);
    Poly r = { qlen, quot };
    return r;
}

/* free a polynomial */
static void poly_free(Poly *p)
{
    if (p->coeff) {
        free(p->coeff);
        p->coeff = NULL;
    }
    p->len = 0;
}

/* global array holding the first 1995 coefficients of the sequence */
static seq_t *period = NULL;
static bool period_init = false;

/* build the period by computing Φ1995(x) and then the recurrence */
static void build_period(void)
{
    /* divisors of 1995 = 3·5·7·19 in increasing order */
    const uint64_t divisors[] = {
        1, 3, 5, 7, 15, 19, 21, 35, 57, 95,
        105, 133, 285, 399, 665, 1995
    };
    const size_t Ndiv = sizeof(divisors)/sizeof(divisors[0]);
    Poly polys[Ndiv];

    /* initialise */
    for (size_t i = 0; i < Ndiv; ++i) {
        polys[i].len = 0;
        polys[i].coeff = NULL;
    }

    /* compute Φ_d for each divisor d */
    for (size_t idx = 0; idx < Ndiv; ++idx) {
        uint64_t d = divisors[idx];
        if (d == 1) {           /* Φ_1(x) = x - 1 */
            polys[idx].len = 2;
            polys[idx].coeff = (seq_t*)calloc(2, sizeof(seq_t));
            polys[idx].coeff[0] = -1;
            polys[idx].coeff[1] = 1;
            continue;
        }

        /* product of all proper divisor polynomials of d */
        Poly prod = { 1, (seq_t*)calloc(1, sizeof(seq_t)) };
        prod.coeff[0] = 1;   /* start with the constant polynomial 1 */
        for (size_t j = 0; j < idx; ++j) {
            if (d % divisors[j] == 0) {
                Poly newp = poly_mul(&prod, &polys[j]);
                poly_free(&prod);
                prod = newp;
            }
        }

        /* numerator polynomial x^d - 1 */
        size_t num_len = d + 1;
        seq_t *numer = (seq_t*)calloc(num_len, sizeof(seq_t));
        numer[0]  = -1;
        numer[d]  = 1;

        /* compute Φ_d as (x^d-1)/prod */
        Poly phi = poly_div(numer, num_len, &prod);
        free(numer);
        poly_free(&prod);
        polys[idx] = phi;
    }

    /* the index of d = 1995 in the divisor array is 15 (the last element) */
    const size_t idx1995 = 15;
    Poly phi1995 = polys[idx1995];
    size_t deg = phi1995.len - 1;          /* degree of Φ_1995 */

    /* compute the first 1995 terms using the recurrence */
    period = (seq_t*)calloc(1995, sizeof(seq_t));
    period[0] = 1;                         /* a(0) = 1 */
    for (size_t k = 1; k < 1995; ++k) {
        seq_t sum = 0;
        size_t lim = (k < deg) ? k : deg;
        for (size_t i = 1; i <= lim; ++i) {
            sum += phi1995.coeff[i] * period[k - i];
        }
        period[k] = -sum;
    }

    /* free unused polynomials */
    for (size_t i = 0; i < Ndiv; ++i) {
        if (i != idx1995)
            poly_free(&polys[i]);
    }
    poly_free(&phi1995);
    period_init = true;
}

/* required sequence function */
seq_t a(idx_t n)
{
    if (!period_init)
        build_period();
    return period[n % 1995];
}
