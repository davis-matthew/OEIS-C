/*
 * A014594  Inverse of 585th cyclotomic polynomial.
 * https://oeis.org/A014594
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

/*  Sequence A014594 – coefficients of the reciprocal of the 585th cyclotomic
    polynomial.  The sequence is periodic with period 585.  Each term is
    the coefficient of x^n in 1/Φ₅₈₅(x).  The values are only -1, 0, or 1. */

#include <stddef.h>
#include <stdint.h>

#define PERIOD 585

static seq_t period_coeffs[PERIOD];
static int period_initialized = 0;

seq_t a(idx_t n)
{
    if (!period_initialized) {
        seq_t a_coeff[PERIOD];
        for (idx_t i = 0; i < PERIOD; ++i) a_coeff[i] = 0;
        a_coeff[0] = 1;   /* start with the constant polynomial 1 */

        /* Divisors d of 585 with non–zero μ(585/d) and their μ values:   */
        /*   (d, μ) = (  3, -1), (  9, +1), ( 15, +1), ( 39, +1),   */
        /*            ( 45, -1), (117, -1), (195, -1), (585, +1).   */
        const idx_t d[8] = {3, 9, 15, 39, 45, 117, 195, 585};
        const int   mu[8] = {-1, 1, 1, 1, -1, -1, -1, 1};

        for (int k = 0; k < 8; ++k) {
            seq_t new_coeff[PERIOD];
            for (idx_t i = 0; i < PERIOD; ++i) new_coeff[i] = a_coeff[i];

            idx_t dd = d[k];
            if (mu[k] == 1) {                  /* multiply by 1/(1 - x^d) */
                for (idx_t i = dd; i < PERIOD; ++i)
                    new_coeff[i] += new_coeff[i - dd];
            } else {                           /* mu == -1 : multiply by (1 - x^d) */
                for (idx_t i = dd; i < PERIOD; ++i)
                    new_coeff[i] -= a_coeff[i - dd];
            }

            for (idx_t i = 0; i < PERIOD; ++i) a_coeff[i] = new_coeff[i];
        }

        for (idx_t i = 0; i < PERIOD; ++i) period_coeffs[i] = a_coeff[i];
        period_initialized = 1;
    }

    return period_coeffs[n % PERIOD];
}
