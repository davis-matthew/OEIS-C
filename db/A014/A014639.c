/*
 * A014639  Inverse of 630th cyclotomic polynomial.
 * https://oeis.org/A014639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the coefficient of x^n in the formal power series
 * 1/Φ_630(x) where Φ_630 is the 630th cyclotomic polynomial.
 * The sequence is periodic with period 630.
 */

#include <stddef.h>
#include <stdbool.h>

/* Period length for this sequence */
#define P 630

/* Static cache for the 630 coefficients.  Will be computed on first call. */
static seq_t cache[P];
static bool cache_initialized = false;

/* Helper to compute the coefficients once. */
static void init_cache(void) {
    const size_t N = P;

    /* Initialise to identity series: 1 + 0·x + 0·x² + … */
    for (size_t i = 1; i < N; ++i) cache[i] = 0;
    cache[0] = 1;

    /* Factors with exponent +1 : (1 – x^d)      (those d correspond to µ(m)=–1) */
    const size_t neg_d[] = {315,210,126,90,21,15,9,6};
    const size_t neg_cnt = sizeof(neg_d)/sizeof(neg_d[0]);

    for (size_t k = 0; k < neg_cnt; ++k) {
        size_t d = neg_d[k];
        for (size_t i = N; i > d; --i) {
            cache[i-1] -= cache[i-1-d];    /* convolution with (1 – x^d) */
        }
    }

    /* Factors with exponent –1 : (1 – x^d)^–1  (those d correspond to µ(m)=+1) */
    const size_t pos_d[] = {630,105,63,45,42,30,18,3};
    const size_t pos_cnt = sizeof(pos_d)/sizeof(pos_d[0]);

    for (size_t k = 0; k < pos_cnt; ++k) {
        size_t d = pos_d[k];
        for (size_t r = 0; r < d; ++r) {
            for (size_t i = r + d; i < N; i += d) {
                cache[i] += cache[i - d];   /* cumulative sum for (1 – x^d)^–1 */
            }
        }
    }

    cache_initialized = true;
}

/* Public function required by the build system. */
seq_t a(idx_t n) {
    if (!cache_initialized) init_cache();
    return cache[(size_t)(n % P)];
}
