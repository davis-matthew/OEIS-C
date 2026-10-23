/*
 * A015003  Inverse of 994th cyclotomic polynomial.
 * https://oeis.org/A015003
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

/* Inverse of 994th cyclotomic polynomial (coefficients of the
   power‑series expansion of 1/Φ₉₉₄(x)).
   The sequence is periodic with period 994.                         */
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static storage for the first 994 terms */
    static seq_t cache[994];
    static bool ready = false;

    if (!ready)
    {
        /* Step 1: compute Φ₁₄(x) = x⁶ - x⁵ + x⁴ - x³ + x² - x + 1   */
        /* coeffs[0] is constant term */
        const seq_t coeff14[7] = { 1, -1, 1, -1, 1, -1, 1 }; /* Φ₁₄ */

        /* Step 2: build Φ₂ₙ₁₀₀(x) = Φ₁₄(x^71) */
        const size_t deg_phi14 = 6;          /* degree of Φ₁₄ */
        const size_t power = 71;
        const size_t deg_num = deg_phi14 * power; /* 6*71 = 426 */
        /* numerator polynomial g has degree 426 */
        seq_t g[427] = {0};
        for (size_t i = 0; i <= deg_phi14; ++i)
            g[i * power] = coeff14[i];

        /* Step 3: divide g by Φ₁₄ to obtain Φ₉₉₄(x) */
        const size_t deg_phi994 = deg_phi14 * power - deg_phi14; /* 420 */
        seq_t phi994[421] = {0};
        /* Denominator coefficients are coeff14 (h[0] = 1). */
        size_t Qlen = deg_num - deg_phi14 + 1; /* 421 */
        for (size_t i = 0; i < Qlen; ++i)
        {
            /* quotient coefficient */
            phi994[i] = g[i]; /* h[0] == 1 */
            /* subtract contribution from higher terms */
            for (size_t j = 1; j <= deg_phi14; ++j)
                g[i + j] -= phi994[i] * coeff14[j];
        }

        /* Step 4: compute first 994 terms of the reciprocal series
           via recurrence Φ(x) * a(x) = 1.                     */
        const size_t period = 994;
        cache[0] = 1; /* constant term φ(0) = 1 */
        for (size_t k = 1; k < period; ++k)
        {
            seq_t sum = 0;
            /* Only up to min(k, deg_phi994) terms are needed */
            size_t max_i = (k <= deg_phi994) ? k : deg_phi994;
            for (size_t i = 1; i <= max_i; ++i)
                sum += phi994[i] * cache[k - i];
            cache[k] = -sum; /* since φ[0] = 1 */
        }

        ready = true;
    }

    /* Return periodic value */
    const idx_t mod = n % 994ULL;
    return cache[(size_t)mod];
}
