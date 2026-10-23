/*
 * A011634  35th cyclotomic polynomial.
 * https://oeis.org/A011634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 35th cyclotomic polynomial
 *            Φ_35(x)
 * Range:     i8 n <= 24, i16 n <= 24, i32 n <= 24, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (coefficients for n ≥ 25 are 0, since Φ_35 has degree 24) */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static seq_t coeffs[25];
static bool initialized = false;

/* Compute the coefficients of Φ_35(x) only once */
static void init_coeffs(void)
{
    /* ---------- construct the numerator (x^35-1)(x-1) ---------- */
    seq_t num[37] = {0};
    num[0]  = (seq_t)(-1);
    num[35] = (seq_t)1;

    /* multiply by (x-1) */
    seq_t tmp[37] = {0};
    for (idx_t i = 0; i <= 35; ++i) {
        seq_t a = num[i];
        if (!a) continue;          /* skip zeros to avoid unnecessary work */
        tmp[i]     += a * (seq_t)(-1); /* coefficient for x^i   */
        tmp[i + 1] += a * (seq_t)1;    /* coefficient for x^(i+1) */
    }
    for (idx_t i = 0; i <= 36; ++i) num[i] = tmp[i];

    /* ---------- construct the denominator (x^5-1)(x^7-1) ---------- */
    seq_t den[13] = {0};
    seq_t p5[6] = {0};
    p5[0] = (seq_t)(-1);
    p5[5] = (seq_t)1;
    seq_t p7[8] = {0};
    p7[0] = (seq_t)(-1);
    p7[7] = (seq_t)1;
    for (idx_t i = 0; i <= 5; ++i) {
        if (!p5[i]) continue;
        for (idx_t j = 0; j <= 7; ++j) {
            if (!p7[j]) continue;
            den[i + j] += p5[i] * p7[j];
        }
    }

    /* ---------- long division: num / den ---------- */
    const idx_t degN = 36;
    const idx_t degD = 12;
    seq_t quotient[25] = {0};

    for (int k = (int)degN; k >= (int)degD; --k) {
        seq_t factor = num[k];
        quotient[(idx_t)(k - degD)] = factor;
        for (idx_t i = 0; i <= degD; ++i) {
            num[(idx_t)(k - degD + i)] -= factor * den[i];
        }
    }

    /* store the obtained coefficients */
    for (idx_t i = 0; i < 25; ++i) coeffs[i] = quotient[i];
}

/* external entry point required by the framework */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_coeffs();
        initialized = true;
    }
    if (n < 25) {
        return coeffs[n];
    }
    return (seq_t)0;
}
