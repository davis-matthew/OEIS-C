/*
 * A015923  Inverse of 1914th cyclotomic polynomial.
 * https://oeis.org/A015923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   compute coefficients of the 1914‑th cyclotomic polynomial
 *            Φ₁₉₁₄(x) via the * product Φ₁₉₁₄(x) = ∏_{d|1914} (x^d -
 *            1)^{μ(d)} (μ = Möbius function). * Then use the convolution a *
 *            Φ₁₉₁₄ = 1 to obtain the periodic sequence * a(n) = coefficients
 *            of the formal power series 1/Φ₁₉₁₄(x)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include only headers that are allowed. */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Period of the sequence. */
    enum { PERIOD = 1914 };

    /* Upper bounds for coefficient arrays. Max degree during
     * multiplication of positive terms is the sum of the positive
     * divisors (2380). Pick a small safety margin. */
    enum { MAX_DEG = 2600 };

    /* Pre‑computed divisors of 1914 with their Möbius μ(d) signs. */
    const int8_t mu[16]  = {  1, -1, -1,  1, -1,  1,  1, -1,
                              -1,  1,  1, -1,  1, -1, -1,  1 };
    const uint16_t divs[16] = {  1,  2,  3,  6, 11, 22, 33, 66,
                                29, 58, 87, 174, 319, 638, 957, 1914 };

    /* Static state that persists between calls. */
    static int      initialized = 0;
    static seq_t   coeff[MAX_DEG];
    static seq_t   tmp[MAX_DEG];
    static seq_t   quot[MAX_DEG];
    static seq_t   val[PERIOD];
    static size_t  deg = 0;           /* current degree of coeff[0..deg] */

    if (!initialized) {
        initialized = 1;

        /* ---- Construct Φ₁₉₁₄(x) via product of (x^d - 1)^μ(d). ---- */

        /* Start with the constant polynomial 1. */
        for (size_t i = 0; i < MAX_DEG; ++i) coeff[i] = 0;
        coeff[0] = 1;
        deg = 0;

        /* First apply all positive μ(d) factors: multiply by (x^d - 1). */
        for (size_t idx = 0; idx < 16; ++idx) {
            if (mu[idx] <= 0) continue;          /* skip negative or zero */
            uint16_t d = divs[idx];
            size_t new_deg = deg + d;

            /* Zero tmp up to new_deg. */
            for (size_t i = 0; i <= new_deg; ++i) tmp[i] = 0;

            /* (x^d - 1) * coeff. */
            for (size_t i = 0; i <= deg; ++i) {
                tmp[i]     -= coeff[i];         /* *(-1)             */
                tmp[i + d] += coeff[i];         /* *x^d              */
            }

            /* Copy back and update degree. */
            for (size_t i = 0; i <= new_deg; ++i) coeff[i] = tmp[i];
            deg = new_deg;
        }

        /* Now divide by all negative μ(d) factors: / (x^d - 1). */
        for (size_t idx = 0; idx < 16; ++idx) {
            if (mu[idx] >= 0) continue;          /* skip positive or zero */
            uint16_t d = divs[idx];
            size_t old_deg = deg;
            size_t new_deg = old_deg - d;

            /* Compute quotient of coeff by (x^d - 1). */
            for (size_t k = 0; k <= new_deg; ++k) {
                if (k < d)
                    quot[k] = - coeff[k];
                else
                    quot[k] = quot[k - d] - coeff[k];
            }

            /* Copy quotient back and zero higher terms. */
            for (size_t i = 0; i <= new_deg; ++i) coeff[i] = quot[i];
            for (size_t i = new_deg + 1; i <= old_deg; ++i) coeff[i] = 0;
            deg = new_deg;
        }

        /* ----------------------------------------------------------------
         * At this point coeff[0..deg] contains Φ₁₉₁₄(x).  Now compute the
         * first PERIOD terms of a(n) by solving a * Φ = 1.
         * ---------------------------------------------------------------- */

        val[0] = 1;
        for (size_t i = 1; i < PERIOD; ++i) {
            seq_t sum = 0;
            size_t kmax = i <= deg ? i : deg;
            for (size_t k = 1; k <= kmax; ++k) {
                if (coeff[k] != 0)
                    sum += coeff[k] * val[i - k];
            }
            val[i] = -sum;
        }
    }

    /* Return the periodic value. */
    return val[(size_t) (n % PERIOD)];
}
