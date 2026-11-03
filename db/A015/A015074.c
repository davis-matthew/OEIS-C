/*
 * A015074  Inverse of 1065th cyclotomic polynomial.
 * https://oeis.org/A015074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the formal power series *
 *            1/Φ1065(x). Φ1065(x) is the 1065th cyclotomic polynomial, of
 *            degree 560. * The coefficients are periodic with period 1065
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Validated: standard, pass, 97 terms through n = 1985 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Helper to compute the coefficients of Φ1065(x) (degree 560). */
static void
compute_phi_coeffs(seq_t coeff[561])
{
    /* start with polynomial equal to 1 */
    static seq_t poly[561];
    for (size_t i = 0; i < 561; ++i) poly[i] = 0;
    poly[0] = 1;

    /* multiplication by the numerator polynomials: (x^d - 1) */
    const size_t numerators[] = {1065, 71, 5, 3};
    for (size_t idx = 0; idx < sizeof(numerators)/sizeof(numerators[0]); ++idx) {
        size_t d = numerators[idx];
        seq_t tmp[561] = {0};
        for (size_t i = 0; i < 561; ++i) {
            seq_t val = -poly[i];
            if (i >= d) val += poly[i - d];
            tmp[i] = val;
        }
        for (size_t i = 0; i < 561; ++i) poly[i] = tmp[i];
    }

    /* division by the denominator polynomials: multiply by the inverse
     * series of (x^d - 1).  The inverse series has coefficient -1 at
     * multiples of d and 0 elsewhere. */
    const size_t denominators[] = {1, 15, 213, 355};
    for (size_t idx = 0; idx < sizeof(denominators)/sizeof(denominators[0]); ++idx) {
        size_t d = denominators[idx];
        seq_t tmp[561] = {0};
        for (size_t i = 0; i < 561; ++i) {
            seq_t s = 0;
            for (size_t m = 0; m * d <= i; ++m)
                s += poly[i - m * d];
            tmp[i] = -s;
        }
        for (size_t i = 0; i < 561; ++i) poly[i] = tmp[i];
    }

    for (size_t i = 0; i < 561; ++i) coeff[i] = poly[i];
}

seq_t a(idx_t n)
{
    /* static cache of one period */
    static struct {
        seq_t period[1065];
        int    ready;
    } cache = { .ready = 0 };

    if (!cache.ready) {
        /* compute coefficients of Φ1065(x) and the inverse series */
        seq_t phi[561];
        compute_phi_coeffs(phi);

        /* a[0] = 1, a[k] = -Sum_{i=1}^{min(k,560)} phi[i]*a[k-i]  */
        cache.period[0] = 1;
        for (size_t k = 1; k < 1065; ++k) {
            seq_t sum = 0;
            size_t upper = k < 560 ? k : 560;
            for (size_t i = 1; i <= upper; ++i)
                sum += phi[i] * cache.period[k - i];
            cache.period[k] = -sum;
        }

        cache.ready = 1;
    }
    return cache.period[(size_t)(n % 1065ULL)];
}
