/*
 * A178795  Expansion of the polynomial
 *          (x^15-1)*(x^12-1)*(x^10-1)*(x^9-1)*(x^7-1)*(x^6-1)*(x^4-1)*(x-1)
 *          in increasing powers of x.
 * https://oeis.org/A178795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 64, i16 n <= 64, i32 n <= 64, i64 n <= 64, i128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Expansion of (x^15-1)*(x^12-1)*(x^10-1)*(x^9-1)*(x^7-1)*\
 * (x^6-1)*(x^4-1)*(x-1) in increasing powers of x.
/* 
 * a(0) = 1
 * a(1) = -1
 * a(2) = 0
 * a(3) = 0
 * ...
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The largest non‑zero term is at x^64, so we only need 65 coefficients. */
    static seq_t coeff[65];
    static bool init = false;

    if (!init) {
        seq_t temp[65];
        idx_t deg = 0;

        /* Start with the polynomial 1. */
        for (idx_t i = 0; i < 65; ++i) coeff[i] = 0;
        coeff[0] = 1;

        /* Exponents of the factors (x^k - 1). */
        const int exps[8] = {15, 12, 10, 9, 7, 6, 4, 1};

        for (int step = 0; step < 8; ++step) {
            idx_t exp = (idx_t)exps[step];
            idx_t new_deg = deg + exp;

            /* Clear temporary buffer. */
            for (idx_t i = 0; i <= new_deg; ++i) temp[i] = 0;

            /* new_coeff[j] = old_coeff[j] * (-1) + old_coeff[j-exp] * 1   */
            for (idx_t i = 0; i <= deg; ++i) {
                temp[i] -= coeff[i];          /* subtract old term */
                temp[i + exp] += coeff[i];    /* add shifted term */
            }

            /* Copy back to coeff array. */
            for (idx_t i = 0; i <= new_deg; ++i) coeff[i] = temp[i];
            deg = new_deg;
        }

        init = true;
    }

    if (n >= 65) return 0;   /* degrees beyond the polynomial give zero */
    return coeff[n];
}
