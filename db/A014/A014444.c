/*
 * A014444  Inverse of 435th cyclotomic polynomial.
 * https://oeis.org/A014444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the power series of 1/Φ_435(x),
 *            where Φ_435(x) is the 435‑th cyclotomic polynomial. The sequence
 *            is periodic with period 435
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required type definitions */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* Helper structure for polynomial operations */
#define MAX_POLY_DEG 500
struct poly {
    seq_t coeff[MAX_POLY_DEG];
    size_t deg;
};

/* Multiply two polynomials A and B, result in R */
static void poly_mul(const struct poly *A, const struct poly *B, struct poly *R)
{
    size_t na = A->deg, nb = B->deg;
    size_t nr = na + nb;
    if (nr >= MAX_POLY_DEG) nr = MAX_POLY_DEG - 1;   /* safe guard */
    memset(R->coeff, 0, sizeof(seq_t) * (na + nb + 1));
    for (size_t i = 0; i <= na; ++i)
        for (size_t j = 0; j <= nb; ++j)
            R->coeff[i + j] += A->coeff[i] * B->coeff[j];
    R->deg = na + nb;
}

/* Exact division of polynomial N by D (leading coefficient of D is 1).
   Result stored in Q. */
static void poly_div(const struct poly *N, const struct poly *D, struct poly *Q)
{
    /* copy numerator to a temporary polynomial */
    struct poly tmp = *N;
    int nd = (int)N->deg, dd = (int)D->deg;
    int qdeg = nd - dd;
    if (qdeg < 0) qdeg = 0;
    memset(Q->coeff, 0, sizeof(seq_t) * (qdeg + 1));
    Q->deg = qdeg;

    for (int i = nd; i >= dd; --i) {
        seq_t coeff = tmp.coeff[i];
        if (coeff != 0) {
            int k = i - dd;
            Q->coeff[k] = coeff;                     /* D has leading 1 */
            for (int j = 0; j <= dd; ++j) {
                tmp.coeff[k + j] -= coeff * D->coeff[j];
            }
        }
    }
    /* trim leading zeros */
    while (Q->deg > 0 && Q->coeff[Q->deg] == 0)
        --Q->deg;
}

/* Compute the Periodic Sequence a(n) */
static seq_t period[435];
static bool initialized = false;

static void init_sequence(void)
{
    /* Divisors of 435 in ascending order */
    const int divs[] = {1, 3, 5, 15, 29, 87, 145, 435};
    const size_t dcnt = sizeof(divs) / sizeof(divs[0]);

    /* Polynomials for each divisor */
    struct poly phi[456] = {0};

    /* Compute Φ_d for all divisors d */
    for (size_t idx = 0; idx < dcnt; ++idx) {
        int d = divs[idx];
        /* Numerator: x^d - 1 */
        struct poly num;
        memset(num.coeff, 0, sizeof(num.coeff));
        num.coeff[0] = -1;
        if (d < (int)MAX_POLY_DEG) num.coeff[d] = 1;
        num.deg = d;

        /* Denominator: product of Φ_e for e | d, e < d */
        struct poly denom;
        memset(denom.coeff, 0, sizeof(denom.coeff));
        denom.coeff[0] = 1;   /* polynomial '1' */
        denom.deg = 0;

        for (size_t j = 0; j < idx; ++j) {          /* only smaller divisors */
            int e = divs[j];
            if (d % e == 0) {
                struct poly tmp;
                poly_mul(&denom, &phi[e], &tmp);
                denom = tmp;   /* copy result back to denom */
            }
        }

        /* φ_d = (x^d - 1) / denom */
        struct poly quo;
        poly_div(&num, &denom, &quo);
        phi[d] = quo;
    }

    /* Now phi[435] holds Φ_435(x) */
    struct poly *c = &phi[435];

    /* Build periodic sequence using linear recurrence */
    period[0] = 1;
    for (size_t k = 1; k < 435; ++k) {
        seq_t sum = 0;
        size_t max_i = c->deg < k ? c->deg : k;
        for (size_t i = 0; i <= max_i; ++i) {
            sum += c->coeff[i] * period[k - i];
        }
        period[k] = -sum;          /* because c[0] == 1 */
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_sequence();
    size_t idx = (size_t)(n % 435ULL);   /* modulo period length */
    return period[idx];
}
