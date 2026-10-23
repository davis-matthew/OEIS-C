/*
 * A015505  Inverse of 1496th cyclotomic polynomial.
 * https://oeis.org/A015505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = coefficient of x^n in the series inverse of the 1496th
 *            cyclotomic polynomial
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   all_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/all_divisors.h"

/* Constants -------------------------------------------------------------- */
#define MAX_DEG 1498     /* accommodates degrees up to 1496 + 1 */
#define MAX_CACHE 20     /* enough for all divisors of 1496 */

/* Helper structures ----------------------------------------------------- */
typedef struct {
    size_t len;                          /* number of coefficients */
    seq_t coeff[MAX_DEG];                /* coefficient array [0..len-1] */
} poly_t;

/* Global cache for cyclotomic polynomials of required divisors --------- */
static uint64_t cache_values[MAX_CACHE];
static poly_t   cache_poly[MAX_CACHE];
static int      cache_count = 0;

/* --------------------------------------------------------------------- */
/* Polynomial multiplication: res = a * b ------------------------------- */
static void poly_mul(const seq_t *a, size_t alen,
                     const seq_t *b, size_t blen,
                     seq_t *res, size_t *reslen) {
    seq_t tmp[MAX_DEG] = {0};
    size_t i, j;

    for (i = 0; i < alen; ++i) {
        if (a[i] == 0) continue;
        for (j = 0; j < blen; ++j) {
            tmp[i + j] += a[i] * b[j];
        }
    }
    size_t len = alen + blen - 1;
    while (len > 1 && tmp[len - 1] == 0) --len;
    *reslen = len;
    for (i = 0; i < len; ++i) res[i] = tmp[i];
}

/* --------------------------------------------------------------------- */
/* Polynomial division: Q = N / D  (exact division, D[highest] == 1) -----*/
static void poly_div(const seq_t *N, size_t Nlen,
                     const seq_t *D, size_t Dlen,
                     seq_t *Q, size_t *Qlen) {
    seq_t R[MAX_DEG];
    size_t i, j;
    for (i = 0; i < Nlen; ++i) R[i] = N[i];
    for (; i < MAX_DEG; ++i) R[i] = 0;

    size_t qlen = Nlen - Dlen + 1;
    for (i = 0; i < qlen; ++i) Q[i] = 0;

    for (i = (size_t)(Nlen - 1); (int)i >= (int)(Dlen - 1); --i) {
        seq_t coeff = R[i];
        if (coeff != 0) {
            size_t offset = i - Dlen + 1;
            Q[offset] = coeff;
            for (j = 0; j < Dlen; ++j) {
                R[offset + j] -= coeff * D[j];
            }
        }
    }

    /* Trim trailing zeros */
    while (qlen > 1 && Q[qlen - 1] == 0) --qlen;
    *Qlen = qlen;
}

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* Retrieve or compute Φ_d(x) stored in cache -------------------------- */
static const poly_t *get_phi(uint64_t d) {
    /* search cache */
    for (int i = 0; i < cache_count; ++i) {
        if (cache_values[i] == d) return &cache_poly[i];
    }

    /* not cached => compute */
    poly_t result;
    if (d == 1) {
        result.len = 2;
        result.coeff[0] = -1;
        result.coeff[1] = 1;
    } else {
        /* product of Φ_e for all proper divisors e of d */
        seq_t B[MAX_DEG] = {0};
        size_t Blen = 1;
        B[0] = 1;                           /* start with 1 */

        uint64_t divs[32];
        int divcnt;
        all_divisors(d, divs, &divcnt);

        for (int idx = 0; idx < divcnt; ++idx) {
            uint64_t e = divs[idx];
            if (e == d) continue;          /* proper divisor only */

            const poly_t *phi_e = get_phi(e);
            seq_t tmp[MAX_DEG];
            size_t tmplen;
            poly_mul(B, Blen, phi_e->coeff, phi_e->len, tmp, &tmplen);
            for (size_t t = 0; t < tmplen; ++t) B[t] = tmp[t];
            Blen = tmplen;
        }

        /* Numerator N(x) = x^d - 1 */
        seq_t N[MAX_DEG] = {0};
        N[0] = -1;
        N[d] = 1;
        size_t Nlen = d + 1;

        /* Perform division N / B to get Φ_d */
        seq_t Q[MAX_DEG];
        size_t Qlen;
        poly_div(N, Nlen, B, Blen, Q, &Qlen);

        result.len = Qlen;
        for (size_t i = 0; i < Qlen; ++i) result.coeff[i] = Q[i];
    }

    /* Store in cache */
    cache_values[cache_count] = d;
    cache_poly[cache_count] = result;
    ++cache_count;
    return &cache_poly[cache_count - 1];
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n) {
    /* lazy initialization */
    static seq_t inv_series[1496];
    static int initialized = 0;

    if (!initialized) {
        /* Precompute Φ_1496 */
        const poly_t *phi1496 = get_phi(1496);

        /* Compute series inverse up to degree 1495 */
        inv_series[0] = 1;
        for (size_t m = 1; m < 1496; ++m) {
            seq_t sum = 0;
            for (size_t k = 1; k <= m; ++k) {
                if (k < phi1496->len) {
                    sum += phi1496->coeff[k] * inv_series[m - k];
                }
            }
            inv_series[m] = -sum;
        }
        initialized = 1;
    }

    return inv_series[n % 1496U];
}
