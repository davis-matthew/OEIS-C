/*
 * A015807  Inverse of 1798th cyclotomic polynomial.
 * https://oeis.org/A015807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: Recurrence from the coefficients of the 1798th cyclotomic polynomial.
 * a(0) = 1
 * a(n) = -∑_{k=1}^{deg} c_k * a(n-k)  (deg = 840, c_k are
 * coefficients of Φ1798(x)), for all n > 0
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Helper functions for polynomial arithmetic (coefficients in seq_t)*/
/* ------------------------------------------------------------------ */

static seq_t *poly_mul(const seq_t *a, size_t alen,
                       const seq_t *b, size_t blen)
{
    size_t len = alen + blen - 1;
    seq_t *res = (seq_t*)calloc(len, sizeof(seq_t));
    for (size_t i = 0; i < alen; ++i) {
        for (size_t j = 0; j < blen; ++j) {
            res[i + j] += a[i] * b[j];
        }
    }
    return res;
}

static seq_t *poly_div(const seq_t *dividend, size_t dlen,
                       const seq_t *divisor, size_t dl2)
{
    /* divisor is monic (leading coefficient 1) */
    size_t qlen = dlen - dl2 + 1;
    seq_t *q = (seq_t*)calloc(qlen, sizeof(seq_t));

    seq_t *rem = (seq_t*)calloc(dlen, sizeof(seq_t));
    memcpy(rem, dividend, dlen * sizeof(seq_t));

    for (int64_t i = (int64_t)dlen - 1; i >= (int64_t)dl2 - 1; --i) {
        seq_t coeff = rem[i];
        size_t qindex = (size_t)(i - (dl2 - 1));
        q[qindex] = coeff;
        for (size_t j = 0; j < dl2; ++j) {
            size_t idx = (size_t)i - (dl2 - 1) + j;
            rem[idx] -= coeff * divisor[j];
        }
    }
    free(rem);
    return q;
}

/* ------------------------------------------------------------------ */
/* Cyclotomic polynomial generation for divisors of 1798               */
/* ------------------------------------------------------------------ */

#define NUM_DIVISORS 8
static const idx_t divisors[NUM_DIVISORS] = {
    1, 2, 29, 31, 58, 62, 899, 1798
};

static seq_t *phi[NUM_DIVISORS];
static size_t phi_len[NUM_DIVISORS];

static void compute_phi_polynomials(void)
{
    /* Φ1(x) = x - 1 */
    phi[0] = (seq_t*)calloc(2, sizeof(seq_t));
    phi[0][0] = -1;
    phi[0][1] = 1;
    phi_len[0] = 2;

    for (int idx = 1; idx < NUM_DIVISORS; ++idx) {
        idx_t d = divisors[idx];

        /* dividend : x^d - 1  => coeff array of length d+1 */
        seq_t *dividend = (seq_t*)calloc((size_t)d + 1, sizeof(seq_t));
        dividend[0] = -1;
        dividend[(size_t)d] = 1;          /* coefficient of x^d */

        /* divisor : product of Φ_e for all e|d, e<d  */
        seq_t *prod = (seq_t*)calloc(1, sizeof(seq_t));
        prod[0] = 1;                      /* polynomial "1" */
        size_t prod_len = 1;

        for (int j = 0; j < idx; ++j) {
            if (d % divisors[j] == 0) {
                seq_t *tmp = poly_mul(prod, prod_len, phi[j], phi_len[j]);
                free(prod);
                prod = tmp;
                prod_len += phi_len[j] - 1;
            }
        }

        /* divide dividend by divisor to get Φ_d */
        seq_t *tmp = poly_div(dividend, (size_t)d + 1, prod, prod_len);
        free(dividend);
        free(prod);

        phi[idx] = tmp;
        phi_len[idx] = (size_t)d + 1 - prod_len + 1; /* degree +1 */
    }
}

/* ------------------------------------------------------------------ */
/* Periodic sequence computation                                     */
/* ------------------------------------------------------------------ */

#define PERIOD 1798
static seq_t period_seq[PERIOD];
static bool initialized = false;

static void init_sequence(void)
{
    compute_phi_polynomials();

    /* Φ_1798 coefficients (LCM indices start at 0) */
    seq_t *root = phi[NUM_DIVISORS - 1];
    size_t root_len = phi_len[NUM_DIVISORS - 1]; /* should be 841 */

    /* recurrence: a[0] = 1, a[n] = - Σ_{k=1}^{min(n,root_len-1)} root[k] * a[n-k] */
    period_seq[0] = 1; /* constant term of Φ is 1 */

    for (idx_t n = 1; n < PERIOD; ++n) {
        seq_t sum = 0;
        size_t kmax = (n < (root_len - 1)) ? (size_t)n : (root_len - 1);
        for (size_t k = 1; k <= kmax; ++k) {
            sum += root[k] * period_seq[(size_t)n - k];
        }
        period_seq[(size_t)n] = -sum;
    }

    /* free resources: phi[0]..phi[NUM_DIVISORS-2] and root */
    for (int i = 0; i < NUM_DIVISORS - 1; ++i) {
        free(phi[i]);
    }
    free(root);

    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Requested function                                                  */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (!initialized) {
        init_sequence();
    }
    return period_seq[(size_t)(n % PERIOD)];
}
