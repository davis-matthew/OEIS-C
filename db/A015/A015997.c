/*
 * A015997  Inverse of 1988th cyclotomic polynomial.
 * https://oeis.org/A015997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   poly_div_monico
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Inverse of the 1988th cyclotomic polynomial.
 * The coefficients of a(n) form a period‑1988 sequence.
 * We compute the coefficients of Φ1988(x) once, then
 * generate the first 1988 terms using the linear recurrence
 * a(n) = - Σ_{k=1}^{φ} coeff[k] * a(n-k)   (n ≥ 1).
 * The recurrence is periodic, so a(n) = a(n mod 1988) for all n.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/poly_div_monico.h"

/* Multiply two polynomials: c = a * b.
 * a has degree da, b has degree db.
 * Returned array c has length da+db+1. */
static seq_t *poly_mul(const seq_t *a, size_t da,
                       const seq_t *b, size_t db,
                       size_t *dout)
{
    size_t d = da + db;
    seq_t *c = (seq_t *)calloc(d + 1, sizeof(seq_t));
    for (size_t i = 0; i <= da; ++i) if (a[i]) {
        for (size_t j = 0; j <= db; ++j) {
            c[i + j] += a[i] * b[j];
        }
    }
    *dout = d;
    return c;
}

seq_t a(idx_t n)
{
    /* One‑time precomputation ------------------------------------------------*/
    static seq_t *coeff = NULL;   /* coefficients of Φ1988 */
    static seq_t *seq = NULL;     /* first 1988 terms of A(x) */
    static int initialized = 0;

    if (!initialized) {
        /* Divisors of 1988 in ascending order. */
        const uint64_t divisors[] = {
            1ULL, 2ULL, 4ULL, 7ULL, 14ULL, 28ULL, 71ULL,
            142ULL, 284ULL, 497ULL, 994ULL, 1988ULL
        };
        const size_t DCOUNT = sizeof(divisors) / sizeof(divisors[0]);

        /* Store each Φ_d(x) */
        seq_t *poly[DCOUNT];
        size_t deg[DCOUNT];

        for (size_t i = 0; i < DCOUNT; ++i) poly[i] = NULL;

        for (size_t idx = 0; idx < DCOUNT; ++idx) {
            uint64_t d = divisors[idx];

            /* Product of Φ_{d'} for all proper divisors d'|d */
            seq_t *product = (seq_t *)calloc(1, sizeof(seq_t));
            product[0] = 1;
            size_t deg_prod = 0;

            for (size_t j = 0; j < idx; ++j) {
                if (d % divisors[j] == 0) {
                    seq_t *new_prod;
                    size_t new_deg;
                    new_prod = poly_mul(product, deg_prod,
                                        poly[j], deg[j], &new_deg);
                    free(product);
                    product = new_prod;
                    deg_prod = new_deg;
                }
            }

            /* Numerator: x^d - 1  */
            size_t dn = (size_t)d;
            seq_t *num = (seq_t *)calloc(dn + 1, sizeof(seq_t));
            num[0] = -1;
            num[dn] = 1;

            /* Φ_d(x) = (x^d - 1) / product_of_proper */
            seq_t *phi;
            size_t dphi;
            phi = poly_div_monico(num, dn, product, deg_prod, &dphi);

            poly[idx] = phi;
            deg[idx]  = dphi;

            free(num);
            free(product); /* product no longer needed */
        }

        /* Φ_1988 is the last polynomial in the list. */
        const size_t LAST = DCOUNT - 1;
        coeff = poly[LAST];
        const size_t PHI_DEG = deg[LAST]; /* 840 */

        /* Compute the first 1988 terms of the sequence. */
        seq = (seq_t *)calloc(1988, sizeof(seq_t));
        seq[0] = 1;
        for (size_t i = 1; i < 1988; ++i) {
            seq_t s = 0;
            size_t kmax = (i < PHI_DEG) ? i : PHI_DEG;
            for (size_t k = 1; k <= kmax; ++k) {
                s += coeff[k] * seq[i - k];
            }
            seq[i] = -s;
        }

        initialized = 1;
    }

    /* Periodic answer ---------------------------------------------------------*/
    idx_t idx = n % 1988ULL;
    return seq[(size_t)idx];
}
