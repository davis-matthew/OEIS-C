/*
 * A016403  407th cyclotomic polynomial.
 * https://oeis.org/A016403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   Φ407(x) = (x^407 − 1) / ((x − 1)·(Φ11(x)·Φ37(x))) The sequence
 *            a(n) is the coefficient of x^n in Φ407(x)
 * Range:     i8 n <= 360, i16 n <= 360, i32 n <= 360, i64 n <= 360,
 *            i128 n <= 360, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Multiply two polynomials a (degree la‑1) and b (degree lb‑1);
   result = a * b.  Assumes monic leading coefficients are not
   required.  Caller must free the returned array. */
static seq_t *poly_mul(const seq_t *a, size_t la,
                      const seq_t *b, size_t lb, size_t *lr)
{
    size_t len = la + lb - 1;
    seq_t *res = (seq_t *)malloc(len * sizeof(seq_t));
    for (size_t i = 0; i < len; ++i) res[i] = 0;
    for (size_t i = 0; i < la; ++i) {
        for (size_t j = 0; j < lb; ++j) {
            res[i + j] += a[i] * b[j];
        }
    }
    *lr = len;
    return res;
}

/* Compute the coefficient array of Φ407(x) once. */
static seq_t *coeffs = NULL;  /* length 361, indices 0..360 */

static void compute_coeffs(void)
{
    /* Φ11(x) = 1 + x + … + x¹⁰ */
    seq_t phi11[11];
    for (size_t i = 0; i < 11; ++i) phi11[i] = 1;
    size_t len_phi11 = 11;

    /* Φ37(x) = 1 + x + … + x³⁶ */
    seq_t phi37[37];
    for (size_t i = 0; i < 37; ++i) phi37[i] = 1;
    size_t len_phi37 = 37;

    /* (x − 1) polynomial */
    seq_t x_minus_1[2] = { -1, 1 };
    size_t len_x_minus_1 = 2;

    /* M(x) = (x − 1)·Φ11(x)·Φ37(x)   (degree 47) */
    size_t len_temp, len_M;
    seq_t *temp = poly_mul(x_minus_1, len_x_minus_1,
                           phi11, len_phi11, &len_temp);   /* 12 coeffs */
    seq_t *M = poly_mul(temp, len_temp, phi37, len_phi37, &len_M); /* 48 coeffs */
    free(temp);

    /* N(x) = x⁴⁰⁷ − 1   (degree 407) */
    size_t len_N = 408;
    seq_t *N = (seq_t *)malloc(len_N * sizeof(seq_t));
    for (size_t i = 0; i < len_N; ++i) N[i] = 0;
    N[0] = -1;
    N[407] = 1;

    /* Quotient Q(x) = N(x) / M(x)   (degree 360) */
    size_t q_len = 361;
    seq_t *Q = (seq_t *)malloc(q_len * sizeof(seq_t));
    for (size_t i = 0; i < q_len; ++i) Q[i] = 0;

    /* Polynomial long division.  M is monic, so the division is trivial. */
    for (long long d = 407; d >= 47; --d) {
        seq_t coef = N[d];
        size_t qi = (size_t)(d - 47);
        Q[qi] = coef;
        if (coef != 0) {
            for (size_t j = 0; j < len_M; ++j) {
                N[qi + j] -= coef * M[j];
            }
        }
    }

    /* Store the coefficients for future calls. */
    coeffs = Q;

    /* Clean up temporary data. */
    free(M);
    free(N);
}

/* Sequence function.  Returns the coefficient of xⁿ in Φ407(x). */
seq_t a(idx_t n)
{
    if (coeffs == NULL) compute_coeffs();
    if ((idx_t)360 < n) {
        return 0;          /* beyond the degree of the polynomial */
    }
    return coeffs[n];
}
