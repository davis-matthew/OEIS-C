/*
 * A015182  Inverse of 1173rd cyclotomic polynomial.
 * https://oeis.org/A015182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in 1/Φ₁₁₇₃(x) where Φ₁₁₇₃(x) is the
 *            1173rd cyclotomic polynomial. Because 1173 = 3·17·23 and all the
 *            prime factors are distinct, Φ₁₁₇₃(x) has the product
 *            representation Φ₁₁₇₃(x) = Π_{d|1173} (xᵈ – 1)^{μ(1173/d)} .
 *            Hence 1/Φ₁₁₇₃(x) = (x‑1)(x⁵¹‑1)(x⁶⁹‑1)(x³͜⁹¹‑1) /((x³‑1)(...
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The series is periodic with period 1173 (Bootle's theorem).   */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#define PERIOD (1173UL)

static seq_t *cache = NULL;   /* coefficients a(0)…a(PERIOD-1) */
static seq_t *work  = NULL;   /* temporary storage */
static bool ready = false;

seq_t a(idx_t n)
{
    /* Build the periodic coefficient array once.                */
    if (!ready) {
        /* Allocate the two arrays once.                          */
        cache = (seq_t *)malloc(PERIOD * sizeof(seq_t));
        work  = (seq_t *)malloc(PERIOD * sizeof(seq_t));
        if (!cache || !work) return (seq_t)0; /* allocation failure */

        /* Initialise to the series 1.                            */
        for (idx_t i = 0; i < PERIOD; ++i) cache[i] = (seq_t)0;
        cache[0] = (seq_t)1;

        /* Multiplication factors: (x^d – 1) with μ(1173/d)=+1.
           They are d = 1, 51, 69, 391.  In the`for` loops the
           indices are taken modulo PERIOD because the result is
           a periodic power series of period PERIOD.                */
        const idx_t mult_d[] = { 1, 51, 69, 391 };
        for (size_t fi = 0; fi < sizeof(mult_d)/sizeof(mult_d[0]); ++fi) {
            idx_t d = mult_d[fi];
            for (idx_t i = 0; i < PERIOD; ++i) {
                /* new[i] = cache[(i + PERIOD - d) % PERIOD] – cache[i];*/
                idx_t j = i + PERIOD - d;
                if (j >= PERIOD) j -= PERIOD;
                work[i] = cache[j] - cache[i];
            }
            /* copy back to cache */
            for (idx_t i = 0; i < PERIOD; ++i) cache[i] = work[i];
        }

        /* Division factors: 1/(x^d – 1) with μ(1173/d)=–1.
           A division by (x^d – 1) corresponds to solving
                old[i] = new[i–d] – new[i]   (new[<0] = 0).
           This recurrence can be resolved linearly from i = 0 upward. */
        const idx_t div_d[] = { 3, 17, 23, 1173 };
        for (size_t fi = 0; fi < sizeof(div_d)/sizeof(div_d[0]); ++fi) {
            idx_t d = div_d[fi];
            for (idx_t i = 0; i < PERIOD; ++i) {
                seq_t prev = (i >= d) ? work[i - d] : (seq_t)0;
                work[i] = prev - cache[i];
            }
            /* copy back to cache */
            for (idx_t i = 0; i < PERIOD; ++i) cache[i] = work[i];
        }

        ready = true;
    }

    /* Return the required term (index modulo PERIOD).          */
    return cache[(size_t)n % PERIOD];
}
