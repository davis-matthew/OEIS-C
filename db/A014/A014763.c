/*
 * A014763  Inverse of 754th cyclotomic polynomial.
 * https://oeis.org/A014763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A014763 – inverse of the 754‑th cyclotomic polynomial.
 *
 *  The generating function is
 *
 *          1/Φ₇₅₄(x) = Σ a(n) xⁿ ,
 *
 *  where Φ₇₅₄(x) is the 754th cyclotomic polynomial.
 *  Φ₇₅₄(x) is monic with constant term 1, so the reciprocal
 *  coefficients a(n) satisfy the linear recurrence
 *
 *          a(0) = 1
 *          a(n) = –∑_{i=1}^{φ(754)} Φ₇₅₄,i · a(n–i)   (n ≥ 1),
 *
 *  where Φ₇₅₄,i is the coefficient of xⁱ in Φ₇₅₄(x) and φ(754)=336.
 *  The sequence is periodic with period 754.  The implementation
 *  precomputes the 754 successive coefficients once and returns the
 *  requested term by indexing the periodic table.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/* Multiply two polynomials A (degree da) and B (degree db).
 *  Result is written into a newly allocated array of length da+db+1.
 *  The caller must free the returned pointer. */
static seq_t *poly_mul(const seq_t *A, size_t da,
                       const seq_t *B, size_t db, size_t *dout)
{
    size_t d = da + db;
    seq_t *C = (seq_t *)calloc(d + 1, sizeof(seq_t));
    if (!C) return NULL;
    for (size_t i = 0; i <= da; ++i)
        if (A[i] != 0)
            for (size_t j = 0; j <= db; ++j)
                C[i + j] += A[i] * B[j];
    *dout = d;
    return C;
}

/* Divide polynomial num (degree dn) by monic polynomial den
 * (degree dd).  The divisor is assumed to divide the dividend
 * exactly.  Quotient is returned in a newly allocated array of
 * length dn-dd+1; *dout is set to the degree of the quotient.
 * The remainder is ignored.  Caller must free the returned pointer. */
static seq_t *poly_div(const seq_t *num, size_t dn,
                       const seq_t *den, size_t dd,
                       size_t *dout)
{
    size_t qlen = dn - dd + 1;
    seq_t *q = (seq_t *)calloc(qlen, sizeof(seq_t));
    seq_t *r = (seq_t *)calloc(dn + 1, sizeof(seq_t));
    if (!q || !r) {
        free(q); free(r);
        return NULL;
    }
    memcpy(r, num, (dn + 1) * sizeof(seq_t));

    for (int i = (int)dn; i >= (int)dd; --i) {
        seq_t coeff = r[i];
        if (coeff != 0) {
            size_t shift = i - dd;
            q[shift] = coeff;
            for (size_t j = 0; j <= dd; ++j)
                r[shift + j] -= coeff * den[j];
        }
    }
    free(r);
    *dout = qlen - 1;
    return q;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    enum { PERIOD = 754 };                /* period of the sequence */
    enum { N = PERIOD };                  /* we need Φ₇₅₄(x) */

    static bool initialized = false;
    static seq_t period_seq[PERIOD];

    if (!initialized) {
        /* Allocate space for all cyclotomic polynomials Φ₁ … Φ₇₅₄. */
        seq_t **phi = (seq_t **)calloc(N + 1, sizeof(seq_t *));
        size_t *phi_deg = (size_t *)calloc(N + 1, sizeof(size_t));

        /* Φ₁(x) = x – 1. */
        seq_t *p1 = (seq_t *)calloc(2, sizeof(seq_t));
        p1[1] = 1;  p1[0] = -1;
        phi[1] = p1;  phi_deg[1] = 1;

        /* Compute Φ_k for k = 2 … N. */
        for (int k = 2; k <= N; ++k) {
            /* Polynomial x^k – 1. */
            seq_t *poly = (seq_t *)calloc(k + 1, sizeof(seq_t));
            poly[k] = 1;  poly[0] = -1;

            /* Product of Φ_d for all proper divisors d of k. */
            seq_t *prod = (seq_t *)calloc(1, sizeof(seq_t));
            prod[0] = 1;
            size_t dprod = 0; /* degree of prod */

            for (int d = 1; d < k; ++d) {
                if (k % d == 0) {
                    size_t newdeg;
                    seq_t *tmp = poly_mul(prod, dprod, phi[d], phi_deg[d], &newdeg);
                    free(prod);
                    prod = tmp;  dprod = newdeg;
                }
            }

            /* Φ_k = (x^k – 1) / prod. */
            size_t dphi;
            seq_t *phi_k = poly_div(poly, k, prod, dprod, &dphi);

            free(prod);
            free(poly);

            phi[k] = phi_k;
            phi_deg[k] = dphi;
        }

        /* Coefficients of Φ_N = Φ₇₅₄. */
        seq_t *phi_N = phi[N];
        size_t deg_N = phi_deg[N];

        /* Build the periodic sequence via the recurrence. */
        period_seq[0] = (seq_t)1;
        for (idx_t k = 1; k < PERIOD; ++k) {
            seq_t sum = 0;
            size_t max_i = (k < (idx_t)deg_N) ? (size_t)k : deg_N;
            for (size_t i = 1; i <= max_i; ++i)
                sum += (seq_t)phi_N[i] * period_seq[k - i];
            period_seq[k] = -sum;
        }

        /* Free temporary tables. */
        for (int k = 1; k <= N; ++k) free(phi[k]);
        free(phi);
        free(phi_deg);

        initialized = true;
    }

    return period_seq[(size_t)(n % PERIOD)];
}
