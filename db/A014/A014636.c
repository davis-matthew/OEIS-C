/*
 * A014636  Inverse of 627th cyclotomic polynomial.
 * https://oeis.org/A014636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of 1/Φ_627(x) (inverse of the 627th
 *            cyclotomic polynomial)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Maximum degree of Φ_d(x) for d ≤ 627 (φ(627)=360). */
#define MAX_N   627
#define MAX_DEG 700

/* Memoisation tables for Φ_d(x).  Only indices up to 627 are required. */
static bool   phi_computed[MAX_N + 1] = {0};
static size_t phi_deg[MAX_N + 1]      = {0};
static seq_t  phi_coeffs[MAX_N + 1][MAX_DEG];

/* Compute Φ_d(x) recursively and store its coefficients in phi_coeffs[d].
   The coefficients are stored in increasing powers: coeff[0] + coeff[1]x + … .
   phi_deg[d] holds the true degree (= φ(d)).  The polynomial is monic, so
   its constant term is 1 (for d>1) and its leading coefficient is 1.        */
static void compute_phi(idx_t d)
{
    if (phi_computed[d] || d == 0) return;

    if (d == 1) {
        /* Φ_1(x) = x - 1 */
        phi_deg[1] = 1;
        phi_coeffs[1][0] = (seq_t)(-1);
        phi_coeffs[1][1] = (seq_t)(1);
        phi_computed[1] = true;
        return;
    }

    /* Multiply all Φ_e(x) for proper divisors e of d.
       prod holds the product polynomial, initially the constant 1. */
    seq_t prod[MAX_DEG] = {0};
    size_t prod_deg = 0;
    prod[0] = (seq_t)1;

    for (idx_t e = 1; e < d; ++e) {
        if (d % e == 0) {
            compute_phi(e);
            size_t deg_e = phi_deg[e];
            seq_t tmp[MAX_DEG] = {0};

            for (size_t i = 0; i <= prod_deg; ++i)
                for (size_t j = 0; j <= deg_e; ++j)
                    tmp[i + j] += prod[i] * phi_coeffs[e][j];

            size_t new_deg = prod_deg + deg_e;
            for (size_t i = 0; i <= new_deg; ++i) prod[i] = tmp[i];
            prod_deg = new_deg;
        }
    }

    /* Q(x) = x^d - 1.  Divide Q by prod to obtain Φ_d(x). */
    seq_t Q[MAX_DEG] = {0};
    Q[0] = (seq_t)(-1);
    Q[d] = (seq_t)(1);

    seq_t R[MAX_DEG];
    for (size_t i = 0; i <= d; ++i) R[i] = Q[i];
    seq_t res[MAX_DEG] = {0};

    for (int i = (int)d; i >= (int)prod_deg; --i) {
        seq_t factor = R[i];
        if (factor != 0) {
            res[i - prod_deg] = factor;
            for (size_t j = 0; j <= prod_deg; ++j)
                R[i - prod_deg + j] -= factor * prod[j];
        }
    }

    /* The true degree of Φ_d(x) is d - prod_deg. */
    phi_deg[d] = d - prod_deg;
    for (size_t i = 0; i <= phi_deg[d]; ++i) phi_coeffs[d][i] = res[i];

    phi_computed[d] = true;
}

/* Storage for the periodic sequence of length 627. */
static bool   seq_computed = false;
static seq_t period_seq[MAX_N] = {0};

seq_t a(idx_t n)
{
    if (!seq_computed) {
        /* Build Φ_627(x) and then the coefficients of its reciprocal. */
        compute_phi((idx_t)627);
        size_t deg = phi_deg[627];
        period_seq[0] = (seq_t)1;          /* a(0) = 1 */

        for (size_t k = 1; k < MAX_N; ++k) {
            seq_t sum = (seq_t)0;
            size_t upper = (k < deg) ? k : deg;
            for (size_t j = 1; j <= upper; ++j)
                sum += phi_coeffs[627][j] * period_seq[k - j];
            period_seq[k] = -sum;
        }
        seq_computed = true;
    }
    return period_seq[n % (idx_t)MAX_N];
}
