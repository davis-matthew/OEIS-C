/*
 * A015779  Inverse of 1770th cyclotomic polynomial.
 * https://oeis.org/A015779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the power‑series inverse of the
 *            1770‑th cyclotomic polynomial Φ_1770(x)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The inverse is given by the product over all divisors d of 1770 of (1 - x^d)^{‑μ(1770/d)}.
   Since the coefficients are periodic with period 1770, we pre‑compute the first 1770
   terms once and return a(n) = coeff[n mod 1770]. */

#include <stdint.h>

#define PERIOD 1770
#define NUM_DIVISORS 16

/* Divisors of 1770 sorted increasingly. */
static const uint64_t divisors[NUM_DIVISORS] = {
    1ULL, 2ULL, 3ULL, 5ULL, 6ULL, 10ULL, 15ULL, 30ULL,
    59ULL, 118ULL, 177ULL, 295ULL, 354ULL, 590ULL, 885ULL, 1770ULL
};

/* For each divisor d we store the exponent = -μ(1770/d).
   When exponent > 0 we multiply by (1 - x^d); when exponent < 0 we multiply by its reciprocal. */
static const int8_t exponent[NUM_DIVISORS] = {
    -1,  1,  1,  1, -1, -1, -1,  1,
     1, -1, -1, -1,  1,  1,  1, -1
};

static seq_t series[PERIOD];
static int initialized = 0;

static void compute_series(void)
{
    /* zero initialise the series */
    for (idx_t i = 1; i < PERIOD; ++i) series[i] = 0;
    series[0] = 1;

    for (int k = 0; k < NUM_DIVISORS; ++k) {
        uint64_t d = divisors[k];
        if (exponent[k] > 0) {                 /* multiply by (1 - x^d) */
            for (idx_t i = PERIOD; i > d; --i) {
                series[i-1] -= series[i-1 - d];
            }
        } else {                                /* multiply by (1 - x^d)^{-1} = 1 + x^d + x^{2d} + … */
            /* tmp holds the new series temporarily */
            seq_t tmp[PERIOD];
            tmp[0] = series[0];
            for (idx_t i = 1; i < PERIOD; ++i) {
                tmp[i] = series[i];
                if (i >= d) tmp[i] += tmp[i - d];
            }
            for (idx_t i = 0; i < PERIOD; ++i) series[i] = tmp[i];
        }
    }
    initialized = 1;
}

seq_t a(idx_t n)
{
    if (!initialized) compute_series();
    return series[n % PERIOD];
}
