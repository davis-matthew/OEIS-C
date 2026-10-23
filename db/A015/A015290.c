/*
 * A015290  Inverse of 1281st cyclotomic polynomial.
 * https://oeis.org/A015290
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

/* Inverse of 1281st cyclotomic polynomial: a(n) = coefficient of xⁿ in 1/Φ₁₂₈₁(x) */

#include <stddef.h>
#include <stdint.h>

/* Degree of Φ₁₂₈₁(x) and its period */
#define DEG 720
#define PERIOD 1281

/* Multiply polynomial by (x^m – 1) */
static void mul_xm_minus1(seq_t *c, idx_t m)
{
    seq_t tmp[DEG + 1];
    for (idx_t i = 0; i <= DEG; ++i) {
        tmp[i] = (i < m) ? (-c[i]) : (c[i - m] - c[i]);
    }
    for (idx_t i = 0; i <= DEG; ++i) c[i] = tmp[i];
}

/* Divide polynomial by (x^m – 1) – the division is exact for our use */
static void div_xm_minus1(seq_t *c, idx_t m)
{
    seq_t tmp[DEG + 1];
    for (idx_t i = 0; i <= DEG; ++i) {
        tmp[i] = (i < m) ? (-c[i]) : (tmp[i - m] - c[i]);
    }
    for (idx_t i = 0; i <= DEG; ++i) c[i] = tmp[i];
}

/* Compute Φ₁₂₈₁(x) coefficients and the periodic recurrence sequence */
static void init_phi_and_period(seq_t phi[DEG + 1], seq_t period[PERIOD])
{
    /* Initialize Φ₁₂₈₁(x) as 1 */
    for (idx_t i = 0; i <= DEG; ++i) phi[i] = 0;
    phi[0] = 1;

    /* Positive Möbius exponents (multiplication) */
    static const idx_t pos_factors[] = {3, 7, 61, 1281};
    const size_t npos = sizeof(pos_factors) / sizeof(pos_factors[0]);
    for (size_t i = 0; i < npos; ++i) mul_xm_minus1(phi, pos_factors[i]);

    /* Negative Möbius exponents (division) */
    static const idx_t neg_factors[] = {1, 21, 183, 427};
    const size_t nneg = sizeof(neg_factors) / sizeof(neg_factors[0]);
    for (size_t i = 0; i < nneg; ++i) div_xm_minus1(phi, neg_factors[i]);

    /* Compute the periodic coefficients a(0)…a(PERIOD-1) */
    period[0] = 1;
    for (idx_t k = 1; k < PERIOD; ++k) {
        seq_t sum = 0;
        idx_t last = (k < DEG) ? k : DEG;
        for (idx_t j = 1; j <= last; ++j) {
            sum += phi[j] * period[k - j];
        }
        period[k] = -sum;
    }
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    static seq_t phi[DEG + 1];
    static seq_t period[PERIOD];
    static int initialized = 0;

    if (!initialized) {
        init_phi_and_period(phi, period);
        initialized = 1;
    }

    idx_t idx = n % PERIOD;
    return period[idx];
}
