/*
 * A015920  Inverse of 1911th cyclotomic polynomial.
 * https://oeis.org/A015920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^(n mod 1911) in the formal series
 *            1/Φ_1911(x)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = -coeff of x^(n mod 1911) in ∏_{d|1911, d<1911} Φ_d(x). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Period length */
#define PERIOD 1911

/* Number of proper divisors of 1911 */
#define DIV_CNT 11

/* Proper divisors of 1911 = 3 * 7^2 * 13, sorted ascending */
static const idx_t divisors[DIV_CNT] = {1, 3, 7, 13, 21, 39, 49, 91, 147, 273, 637};

static seq_t *phi_coeffs[DIV_CNT];
static size_t  phi_len[DIV_CNT];
static seq_t   period[PERIOD];
static int     period_ready = 0;

/* Multiply two polynomials A and B.  Result allocated. */
static seq_t *poly_mul(const seq_t *A, size_t lenA,
                       const seq_t *B, size_t lenB, size_t *lenC)
{
    size_t c_len = lenA + lenB - 1;
    seq_t *C = (seq_t *)malloc(c_len * sizeof(seq_t));
    if (!C) return NULL;
    memset(C, 0, c_len * sizeof(seq_t));

    for (size_t i = 0; i < lenA; ++i)
        for (size_t j = 0; j < lenB; ++j)
            C[i + j] += A[i] * B[j];

    *lenC = c_len;
    return C;
}

/* Divide polynomial A by B (B is monic).  Return quotient. */
static seq_t *poly_div(const seq_t *A, size_t lenA,
                       const seq_t *B, size_t lenB, size_t *lenQ)
{
    size_t degA = lenA - 1;
    size_t degB = lenB - 1;
    size_t q_len = degA - degB + 1;
    seq_t *q = (seq_t *)malloc(q_len * sizeof(seq_t));
    if (!q) return NULL;
    memset(q, 0, q_len * sizeof(seq_t));

    seq_t *tmp = (seq_t *)malloc((degA + 1) * sizeof(seq_t));
    memcpy(tmp, A, (degA + 1) * sizeof(seq_t));

    for (size_t i = degA; ; --i) {
        size_t shift = i - degB;
        seq_t factor = tmp[i];
        q[shift] = factor;
        for (size_t j = 0; j <= degB; ++j)
            tmp[shift + j] -= factor * B[j];
        if (i == degB) break;
    }
    free(tmp);
    *lenQ = q_len;
    return q;
}

/* Build all cyclotomic polynomials Φ_d(x) for the proper divisors. */
static void build_phi(void)
{
    for (size_t idx = 0; idx < DIV_CNT; ++idx) {
        idx_t d = divisors[idx];
        /* Start with x^d - 1 */
        size_t a_len = d + 1;
        seq_t *A = (seq_t *)calloc(a_len, sizeof(seq_t));
        A[0] = -((seq_t)1);          /* constant term -1 */
        A[d] =  ((seq_t)1);          /* x^d term */

        size_t a_len_cur = a_len;
        /* Divide by all smaller divisor's cyclotomic polynomials */
        for (size_t j = 0; j < idx; ++j) {
            if (d % divisors[j] == 0) {
                size_t q_len;
                seq_t *Q = poly_div(A, a_len_cur,
                                    phi_coeffs[j], phi_len[j],
                                    &q_len);
                free(A);
                A = Q;
                a_len_cur = q_len;
            }
        }
        phi_coeffs[idx] = A;
        phi_len[idx] = a_len_cur;
    }
}

/* Compute the period coefficients R(x) = Π Φ_d(x) (d proper divisors). */
static void compute_period(void)
{
    build_phi();

    /* Product of all Φ_d(x) */
    seq_t *prod = (seq_t *)malloc(sizeof(seq_t));
    prod[0] = ((seq_t)1);
    size_t prod_len = 1;

    for (size_t i = 0; i < DIV_CNT; ++i) {
        size_t new_len;
        seq_t *tmp = poly_mul(prod, prod_len,
                              phi_coeffs[i], phi_len[i],
                              &new_len);
        free(prod);
        prod = tmp;
        prod_len = new_len;
    }

    /* Store coefficients in the period array, with sign flipped. */
    memset(period, 0, PERIOD * sizeof(seq_t));
    for (size_t i = 0; i < prod_len && i < PERIOD; ++i)
        period[i] = -prod[i];

    /* Free allocated memory for φ polynomials and product */
    for (size_t i = 0; i < DIV_CNT; ++i)
        free(phi_coeffs[i]);
    free(prod);

    period_ready = 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (!period_ready)
        compute_period();

    idx_t r = n % PERIOD;
    return period[r];
}
