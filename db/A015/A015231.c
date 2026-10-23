/*
 * A015231  Inverse of 1222nd cyclotomic polynomial.
 * https://oeis.org/A015231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1; for n>0: a(n) = -∑_{k=1}^{min(n,552)} φ_k * a(n-k),
 *            where φ_k are the coefficients of the 1222nd cyclotomic
 *            polynomial Φ_1222(x). The sequence is periodic with period 1222
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_DEG 1284          /* 2+13+47+1222 = 1284, final degree after divisions is 552 */

/* Pre‑computed values */
static seq_t phi[553];       /* coefficients of Φ_1222(x)  (degree 552)   */
static seq_t seq_cache[1222];/* sequence values a(0)…a(1221) */
static int initialized = 0;

/* Multiply polynomial 'vec' of degree *deg by (x^d - 1). */
static void mul_by_xd_minus1(seq_t *vec, size_t *deg, size_t d)
{
    size_t old_deg = *deg;
    size_t new_deg = old_deg + d;
    seq_t tmp[MAX_DEG + 1];
    for (size_t i = 0; i <= new_deg; ++i) tmp[i] = 0;
    for (size_t i = 0; i <= old_deg; ++i) {
        tmp[i]           -= vec[i];      /* multiply by -1   */
        tmp[i + d]       += vec[i];      /* multiply by x^d   */
    }
    for (size_t i = 0; i <= new_deg; ++i) vec[i] = tmp[i];
    *deg = new_deg;
}

/* Divide polynomial 'vec' of degree *deg by (x^d - 1).  The division is exact. */
static void div_by_xd_minus1(seq_t *vec, size_t *deg, size_t d)
{
    size_t old_deg = *deg;
    size_t new_deg = old_deg - d;
    seq_t quot[MAX_DEG + 1];
    for (size_t i = 0; i <= new_deg; ++i) quot[i] = 0;

    for (long long i = (long long)old_deg; i >= (long long)d; --i) {
        seq_t coeff = vec[i];
        quot[i - d] = coeff;
        if (coeff != 0) vec[i - d] += coeff;   /* because substracting -coeff = +coeff */
        vec[i] = 0;
        if (i == (long long)d) break;
    }

    for (size_t i = 0; i <= new_deg; ++i) vec[i] = quot[i];
    *deg = new_deg;
}

/* Compute the coefficients of Φ_1222(x). */
static void compute_phi(void)
{
    seq_t prod[MAX_DEG + 1] = {0};
    size_t deg = 0;
    prod[0] = 1;                     /* start with 1 */

    /* Positive exponent divisors: d = 2, 13, 47, 1222  (μ = +1) */
    mul_by_xd_minus1(prod, &deg, 2);
    mul_by_xd_minus1(prod, &deg, 13);
    mul_by_xd_minus1(prod, &deg, 47);
    mul_by_xd_minus1(prod, &deg, 1222);

    /* Negative exponent divisors: d = 1, 26, 94, 611  (μ = -1) */
    div_by_xd_minus1(prod, &deg, 1);
    div_by_xd_minus1(prod, &deg, 26);
    div_by_xd_minus1(prod, &deg, 94);
    div_by_xd_minus1(prod, &deg, 611);

    /* Copy the resulting polynomial coefficients.  deg should now be 552. */
    for (size_t i = 0; i <= deg; ++i) phi[i] = prod[i];
}

/* Compute the periodic sequence a(0)…a(1221). */
static void compute_seq(void)
{
    seq_cache[0] = 1;
    for (idx_t n = 1; n < 1222; ++n) {
        seq_t sum = 0;
        for (size_t k = 1; k <= 552 && k <= (size_t)n; ++k)
            sum += phi[k] * seq_cache[n - k];
        seq_cache[n] = -sum;
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        initialized = 1;
        compute_phi();
        compute_seq();
    }
    return seq_cache[(size_t)(n % 1222)];
}
