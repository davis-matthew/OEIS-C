/*
 * A014932  Inverse of 923rd cyclotomic polynomial.
 * https://oeis.org/A014932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1; for n≥1: a(n)=−∑_{i=1}^{840} Φ_{923,i}*a(n−i), where
 *            Φ_{923,i} are the coefficients of Φ_923(x). The sequence repeats
 *            with period 923
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A014932: Inverse of 923rd cyclotomic polynomial. */
/* The implementation computes one period by dividing the polynomials
   for Φ_{13·71}(x). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t period[923];
static bool period_initialised = false;

static void init_period(void)
{
    /* constants for 923 = 13·71 */
    const size_t P = 13;
    const size_t Q = 71;
    const size_t deg_den = P - 1;               /* 12 */
    const size_t deg_num = (P - 1) * Q;         /* 852 */
    const size_t deg_phi = (P - 1) * (Q - 1);   /* 840 */
    const size_t period_len = 923;

    /* polynomial coefficients */
    seq_t num[deg_num + 1];
    seq_t denom[deg_den + 1];
    seq_t temp[deg_num + 1];
    seq_t phi[deg_phi + 1];

    /* initialise all to zero */
    for (size_t i = 0; i <= deg_num; ++i) {
        num[i] = 0;
        temp[i] = 0;
    }
    for (size_t i = 0; i <= deg_den; ++i) denom[i] = 0;
    for (size_t i = 0; i <= deg_phi; ++i) phi[i] = 0;

    /* numerator: x^(0·Q) + x^(1·Q) + … + x^((P‑1)·Q) */
    for (size_t j = 0; j <= P - 1; ++j) num[j * Q] = (seq_t)1;

    /* denominator: 1 + x + … + x^(P‑1) */
    for (size_t i = 0; i <= deg_den; ++i) denom[i] = (seq_t)1;

    /* copy numerator for long division */
    for (size_t i = 0; i <= deg_num; ++i) temp[i] = num[i];

    /* long division: temp / denom → phi */
    for (int i = (int)deg_num; i >= (int)deg_den; --i) {
        seq_t coef = temp[i];
        if (coef != (seq_t)0) {
            size_t k = (size_t)i - deg_den;      /* exponent in quotient */
            phi[k] = coef;
            /* subtract coef * denom shifted by k */
            for (size_t j = 0; j <= deg_den; ++j) {
                temp[k + j] = temp[k + j] - coef * denom[j];
            }
        }
    }

    /* recurrence to generate the period */
    period[0] = (seq_t)1;
    for (size_t n = 1; n < period_len; ++n) {
        seq_t sum = 0;
        for (size_t i = 1; i <= deg_phi; ++i) {
            if (phi[i] != (seq_t)0 && n >= i) {
                sum = sum + phi[i] * period[n - i];
            }
        }
        period[n] = -sum;
    }

    period_initialised = true;
}

seq_t a(idx_t n)
{
    if (!period_initialised) init_period();
    const size_t period_len = 923;
    size_t idx = (size_t)(n % period_len);
    return period[idx];
}
