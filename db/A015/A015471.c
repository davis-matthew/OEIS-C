/*
 * A015471  Inverse of 1462nd cyclotomic polynomial.
 * https://oeis.org/A015471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the formal power series *
 *            expansion of 1/Φ1462(x), which is periodic with period 1462. The
 *            term * a(n) equals the n‑th coefficient of the inverse of the
 *            1462‑th * cyclotomic polynomial modulo x^1462
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* The constant divisor list of 1462 = 2·17·43. */
static const idx_t divs[8] = {1, 2, 17, 34, 43, 86, 731, 1462};
#define NUM_DIV 8
#define MAX_N 1462
/* The degree φ(1462) = 672. */
#define MAX_DEG 672

/* Storage for each cyclotomic polynomial Φ_d, d in divs. */
static seq_t phi[NUM_DIV][MAX_DEG + 1];
static size_t deg_phi[NUM_DIV];

/* The inverse series coefficients of Φ_*1462* modulo x^1462. */
static seq_t inv_coeff[MAX_N];
static bool inv_ready = false;

/* Multiply two polynomials a[0..da], b[0..db] into c[0..da+db]. */
static void poly_mul(const seq_t *a, size_t da,
                     const seq_t *b, size_t db,
                     seq_t *c, size_t *dc)
{
    *dc = da + db;
    for (size_t i = 0; i <= *dc; ++i) c[i] = 0;
    for (size_t i = 0; i <= da; ++i) {
        for (size_t j = 0; j <= db; ++j) {
            c[i + j] += a[i] * b[j];
        }
    }
}

/* Exact division of monic polynomial num[0..dn] by den[0..dd],
 * storing quotient in quo[0..dq] (dq = dn - dd). */
static void poly_divexact(const seq_t *num, size_t dn,
                          const seq_t *den, size_t dd,
                          seq_t *quo)
{
    size_t dq = dn - dd;
    seq_t tmp[MAX_N + 1];
    for (size_t i = 0; i <= dn; ++i) tmp[i] = num[i];

    for (int i = (int)dn; i >= (int)dd; --i) {
        seq_t coeff = tmp[i];             /* den[dd] == 1  (monic) */
        quo[i - dd] = coeff;
        if (coeff != 0) {
            for (size_t j = 0; j <= dd; ++j) {
                tmp[i - dd + j] -= coeff * den[j];
            }
        }
    }
}

/* Build all Φ_d for d in divs. */
static void build_cyclotomics(void)
{
    /* Φ_1(x) = x - 1 */
    phi[0][0] = -1;
    phi[0][1] = 1;
    deg_phi[0] = 1;

    for (size_t idx = 1; idx < NUM_DIV; ++idx) {
        idx_t d = divs[idx];

        /* x^d - 1 */
        seq_t b[MAX_N + 1] = {0};
        b[0] = -1;
        b[d] = 1;

        /* product of Φ_m for m|d, m<d */
        seq_t prod[MAX_N + 1] = {0};
        size_t deg_prod = 0;
        prod[0] = 1;   /* polynomial 1 */

        for (size_t j = 0; j < idx; ++j) {
            if (d % divs[j] == 0) {
                seq_t mult[MAX_N + 1] = {0};
                size_t mult_deg = deg_prod + deg_phi[j];
                /* multiply prod by φ[divs[j]] */
                for (size_t a = 0; a <= deg_prod; ++a) {
                    for (size_t b2 = 0; b2 <= deg_phi[j]; ++b2) {
                        mult[a + b2] += prod[a] * phi[j][b2];
                    }
                }
                /* update prod */
                for (size_t k = 0; k <= mult_deg; ++k) prod[k] = mult[k];
                deg_prod = mult_deg;
            }
        }

        /* Divide (x^d - 1) by the product to get Φ_d */
        seq_t q[MAX_N + 1] = {0};
        poly_divexact(b, d, prod, deg_prod, q);
        size_t deg_q = d - deg_prod;

        for (size_t k = 0; k <= deg_q; ++k) phi[idx][k] = q[k];
        deg_phi[idx] = deg_q;
    }
}

/* Compute the inverse series of Φ_1462 modulo x^1462. */
static void compute_inverse(void)
{
    /* Build cyclotomics if not already done. */
    if (deg_phi[0] == 0) build_cyclotomics();

    /* The 1462‑th cyclotomic is phi[NUM_DIV-1]. */
    const seq_t *a = phi[NUM_DIV - 1];
    size_t a_deg = deg_phi[NUM_DIV - 1];

    /* Compute coefficients of 1/Φ_1462 modulo x^1462. */
    inv_coeff[0] = 1;
    for (size_t k = 1; k < MAX_N; ++k) {
        seq_t sum = 0;
        size_t maxj = (k <= a_deg) ? k : a_deg;
        for (size_t j = 1; j <= maxj; ++j) {
            sum += a[j] * inv_coeff[k - j];
        }
        inv_coeff[k] = -sum;   /* a[0] == 1, so division trivial */
    }
    inv_ready = true;
}

seq_t a(idx_t n)
{
    if (!inv_ready) compute_inverse();
    size_t idx = (size_t)n % MAX_N;
    return inv_coeff[idx];
}
