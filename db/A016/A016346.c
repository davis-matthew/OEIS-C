/*
 * A016346  175th cyclotomic polynomial.
 * https://oeis.org/A016346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 175th cyclotomic polynomial
 *            Φ175(x)
 * Range:     i8 n <= 120, i16 n <= 120, i32 n <= 120, i64 n <= 120,
 *            i128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = 0, a(2) = 0, a(3) = 0, a(4) = 0, a(5) = -1, ...  */
/* For n > φ(175) the coefficient is 0.                                */

#include <stdlib.h>
#include <stddef.h>

/* Helper for multiplying two polynomials over seq_t. */
static seq_t *poly_mul(const seq_t *a, size_t na, const seq_t *b, size_t nb, size_t *nres)
{
    size_t nc = na + nb - 1;
    seq_t *c = (seq_t *)malloc(sizeof(seq_t) * nc);
    if (!c) return NULL;
    for (size_t i = 0; i < nc; ++i) c[i] = 0;
    for (size_t i = 0; i < na; ++i)
        for (size_t j = 0; j < nb; ++j)
            c[i + j] += a[i] * b[j];
    *nres = nc;
    return c;
}

/* Helper for dividing a monic polynomial divisor from a dividend.  */
static seq_t *poly_div(const seq_t *num, size_t nnum, const seq_t *den, size_t nden, size_t *nres)
{
    if (nden == 0) return NULL; /* invalid */
    size_t deg_num = nnum - 1;
    size_t deg_den = nden - 1;
    if (deg_num < deg_den) {
        *nres = 0;
        return NULL;
    }
    size_t qdeg = deg_num - deg_den;
    size_t qlen = qdeg + 1;
    seq_t *q = (seq_t *)malloc(sizeof(seq_t) * qlen);
    if (!q) return NULL;
    for (size_t i = 0; i < qlen; ++i) q[i] = 0;

    /* remainder starts as copy of dividend */
    seq_t *rem = (seq_t *)malloc(sizeof(seq_t) * nnum);
    if (!rem) { free(q); return NULL; }
    for (size_t i = 0; i < nnum; ++i) rem[i] = num[i];

    for (int i = (int)qdeg; i >= 0; --i) {
        seq_t coeff = rem[deg_den + i];  /* leading coeff */
        q[i] = coeff;
        /* subtract coeff * den shifted by i */
        for (size_t j = 0; j < nden; ++j) {
            rem[i + j] -= coeff * den[j];
        }
    }
    /* remainder should all be zero; ignore check */
    free(rem);
    *nres = qlen;
    return q;
}

/* Computes the coefficients of Φ175.  */
static void compute_phi175(seq_t *coeff)
{
    /* Φ175 has degree φ(175)=120.  */
    const idx_t n = 175;
    /* List of divisors of 175 in ascending order. */
    const idx_t divs[] = {1,5,7,25,35,175};
    const size_t divs_cnt = sizeof(divs)/sizeof(divs[0]);

    /* Store intermediate polynomials in arrays of pointers. */
    seq_t *poly[divs_cnt];
    size_t len[divs_cnt];

    /* Compute Φ1 = x - 1. */
    poly[0] = (seq_t *)malloc(2 * sizeof(seq_t));
    poly[0][0] = -1;  /* constant term */
    poly[0][1] = 1;   /* x term */
    len[0] = 2;

    /* Compute subsequent Φd using Φd = (x^d - 1) / Π Φj, j|d, j<d */
    for (size_t idx = 1; idx < divs_cnt; ++idx) {
        idx_t d = divs[idx];
        /* Numerator polynomial: x^d - 1. */
        size_t numer_len = d + 1;
        seq_t *numer = (seq_t *)malloc(sizeof(seq_t) * numer_len);
        for (size_t i = 0; i < numer_len; ++i) numer[i] = 0;
        numer[0] = -1;
        numer[d] = 1;

        /* Denominator polynomial: product of all Φj for j|d, j<d. */
        seq_t *den = (seq_t *)malloc(sizeof(seq_t) * 1);
        den[0] = 1;
        size_t den_len = 1;
        for (size_t j = 0; j < idx; ++j) {
            if (d % divs[j] == 0) {
                size_t new_len;
                seq_t *tmp = poly_mul(den, den_len, poly[j], len[j], &new_len);
                free(den);
                den = tmp;
                den_len = new_len;
            }
        }
        size_t res_len;
        seq_t *res = poly_div(numer, numer_len, den, den_len, &res_len);

        poly[idx] = res;
        len[idx] = res_len;

        free(numer);
        free(den);
    }

    /* Φ175 is the last polynomial.  Copy into coeff array. */
    for (size_t i = 0; i < len[divs_cnt - 1]; ++i)
        coeff[i] = poly[divs_cnt - 1][i];
    /* Pad remaining with zeros (degree is 120). */
    for (size_t i = len[divs_cnt - 1]; i <= 120; ++i)
        coeff[i] = 0;

    /* Free intermediate polynomials. */
    for (size_t k = 0; k < divs_cnt; ++k)
        free(poly[k]);
}

seq_t a(idx_t n)
{
    static seq_t coeff[121];
    static int ready = 0;
    if (!ready) {
        compute_phi175(coeff);
        ready = 1;
    }
    if (n <= 120) return coeff[n];
    return 0;
}
