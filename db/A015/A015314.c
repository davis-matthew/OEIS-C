/*
 * A015314  Inverse of 1305th cyclotomic polynomial.
 * https://oeis.org/A015314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = coefficient of x^n in the power series expansion of 1/Φ_1305(x),
 * which equals - coefficient of x^(n mod 1305) in the polynomial
 *   Q(x) = (x^1305 - 1)/Φ_1305(x) = ∏_{d|1305, d<1305} Φ_d(x)
 * where Φ_d are the cyclotomic polynomials.  The sequence is periodic with
 * period 1305, and a(n) = 0 for n mod 1305 > 633, the degree of Q.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Multiply polynomials a (degree na-1) and b (degree nb-1).
   Returns new array of length na+nb-1 with the product coefficients. */
static seq_t *poly_mul(const seq_t *a, size_t na,
                       const seq_t *b, size_t nb,
                       size_t *nout)
{
    size_t len = na + nb - 1;
    seq_t *res = (seq_t*)calloc(len, sizeof(seq_t));
    for (size_t i = 0; i < na; ++i)
        for (size_t j = 0; j < nb; ++j)
            res[i + j] += a[i] * b[j];
    *nout = len;
    return res;
}

/* Divide polynomial a (degree na-1) by monic polynomial b (degree nb-1).
   Returns quotient array of length na-nb+1.  Assumes division is exact. */
static seq_t *poly_div(const seq_t *a, size_t na,
                       const seq_t *b, size_t nb,
                       size_t *nout)
{
    seq_t *rem = (seq_t*)malloc(na * sizeof(seq_t));
    memcpy(rem, a, na * sizeof(seq_t));

    size_t qlen = na - nb + 1;
    seq_t *quo = (seq_t*)calloc(qlen, sizeof(seq_t));

    for (int i = (int)na - 1; i >= (int)nb - 1; --i) {
        seq_t coeff = rem[i];
        size_t pos = i - nb + 1;
        quo[pos] = coeff;
        if (coeff) {
            for (size_t j = 0; j < nb; ++j)
                rem[pos + j] -= coeff * b[j];
        }
    }
    free(rem);
    *nout = qlen;
    return quo;
}

/* Precomputed proper divisors of 1305 (excluding 1305 itself). */
static const idx_t _divs[11] = {1,3,5,9,15,29,45,87,145,261,435};

/* The main sequence function. */
seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t *Q = NULL;     /* coefficients of Q(x) = (x^1305-1)/Φ_1305(x) */
    static size_t Qlen = 0;     /* number of coefficients in Q */

    if (!init) {
        /* Compute cyclotomic polynomials Φ_d for each proper divisor d. */
        const size_t dcnt = 11;
        seq_t *phi[dcnt];
        size_t plen[dcnt];

        /* Φ_1(x) = x - 1 */
        seq_t *phi0 = (seq_t*)calloc(2, sizeof(seq_t));
        phi0[0] = -1;
        phi0[1] = 1;
        phi[0] = phi0;
        plen[0] = 2;

        /* Compute Φ_d for d = 3,5,9,15,29,45,87,145,261,435. */
        for (size_t i = 1; i < dcnt; ++i) {
            idx_t d = _divs[i];

            /* product of Φ_k for k dividing d and k < d */
            size_t prodlen = 1;
            seq_t *prod = (seq_t*)calloc(1, sizeof(seq_t));
            prod[0] = 1;

            for (size_t j = 0; j < i; ++j) {
                if (d % _divs[j] == 0) {
                    size_t newlen;
                    seq_t *tmp = poly_mul(prod, prodlen, phi[j], plen[j], &newlen);
                    free(prod);
                    prod = tmp;
                    prodlen = newlen;
                }
            }

            /* Φ_d(x) = (x^d - 1) / prod */
            size_t alen = (size_t)d + 1;
            seq_t *A = (seq_t*)calloc(alen, sizeof(seq_t));
            A[0] = -1;
            A[d] = 1;

            size_t qlen;
            seq_t *Phi = poly_div(A, alen, prod, prodlen, &qlen);

            free(A);
            free(prod);

            phi[i] = Phi;
            plen[i] = qlen;
        }

        /* Q(x) = product of all Φ_d for proper divisors. */
        size_t Qlen_temp = 1;
        seq_t *Qtemp = (seq_t*)calloc(1, sizeof(seq_t));
        Qtemp[0] = 1;

        for (size_t i = 0; i < dcnt; ++i) {
            size_t newlen;
            seq_t *newQ = poly_mul(Qtemp, Qlen_temp, phi[i], plen[i], &newlen);
            free(Qtemp);
            Qtemp = newQ;
            Qlen_temp = newlen;
        }

        Q = Qtemp;
        Qlen = Qlen_temp;

        /* free φ_d polynomials */
        for (size_t i = 0; i < dcnt; ++i)
            free(phi[i]);

        init = true;
    }

    /* Periodic with period 1305.  a(n) = -Q[n mod 1305] if within degree, else 0. */
    idx_t r = n % 1305;
    if (r < Qlen)
        return -Q[r];
    else
        return 0;
}
