/*
 * A015206  Inverse of 1197th cyclotomic polynomial.
 * https://oeis.org/A015206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   A015206 – inverse of the 1197th cyclotomic polynomial. The
 *            sequence is periodic with period 1197. It satisfies a(0)=1 and
 *            for n>0 a(n) = -∑_{k=1}^{648} c_k * a(n-k), where c_k are the
 *            integer coefficients of Φ₁₁₉₇(x)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Validated: standard, pass, 97 terms through n = 1995 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Helper: compute the coefficients of Φ₁₁₉₇(x).
   coeff[1..648] will be filled; coeff[0] remains unused. */
static void compute_cyclotomic_coeffs(seq_t *coeff)
{
    /* Divisors of 1197=3²·7·19 with μ(n/d)≠0 */
    const size_t plus_d[]   = {9, 21, 57, 1197};  /* μ>0   */
    const size_t minus_d[]  = {3, 63, 171, 399};  /* μ<0   */
    const size_t NUM_PLUS   = sizeof(plus_d) / sizeof(plus_d[0]);
    const size_t NUM_MINUS  = sizeof(minus_d) / sizeof(minus_d[0]);

    /* Degrees of the numerator and denominator polynomials. */
    const size_t NUM_NUM_DEG = plus_d[0] + plus_d[1] + plus_d[2] + plus_d[3];
    const size_t NUM_DEN_DEG = minus_d[0] + minus_d[1] + minus_d[2] + minus_d[3];
    const size_t NUM_PH_DEG = 648; /* φ(1197) */

    /* Temporary arrays (fixed size). */
    seq_t num[NUM_NUM_DEG + 1];
    seq_t den[NUM_DEN_DEG + 1];
    seq_t tmp[NUM_NUM_DEG + 1];

    /* Initialize numerator to 1. */
    for (size_t i = 0; i <= NUM_NUM_DEG; ++i) num[i] = 0;
    num[0] = 1;
    size_t deg_num = 0;

    /* Build numerator: product of (x^d - 1) for μ>0. */
    for (size_t f = 0; f < NUM_PLUS; ++f) {
        size_t d = plus_d[f];
        size_t new_deg = deg_num + d;
        for (size_t i = 0; i <= NUM_NUM_DEG; ++i) tmp[i] = 0;
        for (size_t i = 0; i <= deg_num; ++i) {
            tmp[i]     -= num[i];      /* multiply by -1 */
            tmp[i + d] += num[i];      /* multiply by x^d */
        }
        for (size_t i = 0; i <= new_deg; ++i) num[i] = tmp[i];
        deg_num = new_deg;
    }

    /* Build denominator: product of (x^d - 1) for μ<0. */
    for (size_t i = 0; i <= NUM_DEN_DEG; ++i) den[i] = 0;
    den[0] = 1;
    size_t deg_den = 0;
    for (size_t f = 0; f < NUM_MINUS; ++f) {
        size_t d = minus_d[f];
        size_t new_deg = deg_den + d;
        for (size_t i = 0; i <= NUM_NUM_DEG; ++i) tmp[i] = 0;
        for (size_t i = 0; i <= deg_den; ++i) {
            tmp[i]     -= den[i];
            tmp[i + d] += den[i];
        }
        for (size_t i = 0; i <= new_deg; ++i) den[i] = tmp[i];
        deg_den = new_deg;
    }

    /* Polynomial division: num / den  (both monic). */
    seq_t phi[NUM_PH_DEG + 1];
    for (size_t i = 0; i <= NUM_PH_DEG; ++i) phi[i] = 0;

    for (int i = (int)deg_num; i >= (int)deg_den; --i) {
        seq_t q = num[i];
        phi[i - (int)deg_den] = q;
        for (size_t j = 0; j <= deg_den; ++j) {
            num[i - (int)deg_den + j] -= q * den[j];
        }
    }

    /* Store the polynomial coefficients (c_1 … c_648). */
    for (size_t k = 1; k <= NUM_PH_DEG; ++k) coeff[k] = phi[k];
}

/* Period of the sequence. */
#define PERIOD 1197

/* Main function implementing A015206. */
seq_t a(idx_t n)
{
    static int initialized = 0;
    static seq_t cache[PERIOD];
    static seq_t coeff[649]; /* coeff[1..648] are used; coeff[0] unused */

    if (!initialized) {
        /* Precompute Φ₁₁₉₇ coefficients. */
        compute_cyclotomic_coeffs(coeff);

        /* Build the periodic sequence using the recurrence. */
        cache[0] = 1;                 /* a(0) = 1 */
        for (idx_t i = 1; i < PERIOD; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= 648 && i >= k; ++k) {
                sum += coeff[k] * cache[i - k];
            }
            cache[i] = -sum;
        }
        initialized = 1;
    }
    return cache[n % PERIOD];
}
