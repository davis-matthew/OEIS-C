/*
 * A015599  Inverse of 1590th cyclotomic polynomial.
 * https://oeis.org/A015599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the power series of
 *            1/Φ_1590(x)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   div_shift_sub, mul_shift_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/div_shift_sub.h"
#include "../utils/mul_shift_sub.h"

/* Constants for N = 1590 = 2 * 3 * 5 * 53. */
static const idx_t N = 1590;
static const idx_t PRIMES[4] = {2, 3, 5, 53};
static const size_t NUM_PRIMES = 4;
static const size_t PERIOD = 1590;   /* sequence period */

static seq_t *period = NULL;
static bool initialized = false;

seq_t a(idx_t n) {
    if (!initialized) {
        /* Build Φ_N(x) = ∏_{d|N} (x^{N/d} - 1)^{μ(d)}. */
        seq_t *coeff = (seq_t *)calloc(1, sizeof(seq_t));
        coeff[0] = (seq_t)1;
        size_t deg = 0;

        /* Multiply by factors with μ(d) = +1 (even number of primes). */
        for (int mask = 0; mask < (1 << NUM_PRIMES); ++mask) {
            idx_t d = 1;
            int bits = 0;
            for (int b = 0; b < (int)NUM_PRIMES; ++b) {
                if (mask & (1 << b)) {
                    d *= PRIMES[b];
                    ++bits;
                }
            }
            if ((bits & 1) == 0) {          /* μ(d) = +1 */
                size_t shift = (size_t)(N / d);
                mul_shift_sub(&coeff, &deg, shift);
            }
        }

        /* Divide by factors with μ(d) = -1 (odd number of primes). */
        for (int mask = 0; mask < (1 << NUM_PRIMES); ++mask) {
            idx_t d = 1;
            int bits = 0;
            for (int b = 0; b < (int)NUM_PRIMES; ++b) {
                if (mask & (1 << b)) {
                    d *= PRIMES[b];
                    ++bits;
                }
            }
            if ((bits & 1) == 1) {          /* μ(d) = -1 */
                size_t shift = (size_t)(N / d);
                div_shift_sub(&coeff, &deg, shift);
            }
        }

        /* Compute the first PERIOD coefficients of 1/Φ_N(x). */
        seq_t *tmp = (seq_t *)calloc(PERIOD, sizeof(seq_t));
        tmp[0] = (seq_t)1;
        for (size_t k = 1; k < PERIOD; ++k) {
            seq_t sum = (seq_t)0;
            size_t max_i = deg < k ? deg : k;
            for (size_t i = 1; i <= max_i; ++i) {
                sum += coeff[i] * tmp[k - i];
            }
            tmp[k] = -sum;
        }

        /* Store the periodic sequence. */
        period = (seq_t *)malloc(PERIOD * sizeof(seq_t));
        for (size_t i = 0; i < PERIOD; ++i) {
            period[i] = tmp[i];
        }

        free(tmp);
        free(coeff);
        initialized = true;
    }

    return period[(size_t)(n % PERIOD)];
}
