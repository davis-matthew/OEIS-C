/*
 * A014218  Inverse of 209th cyclotomic polynomial.
 * https://oeis.org/A014218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   compute_coeffs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0) = 1
   For n ≥ 1,
        a(n) = – Σ_{k=1}^{180} c_k · a(n–k),
   where Φ_209(x) = Σ_{k=0}^{180} c_k x^k is the 209th cyclotomic
   polynomial (c_0 = 1).  The sequence is periodic with period 209. */

#include <stddef.h>   /* size_t */
#include "../utils/compute_coeffs.h"

/* ------------------------------------------------------------------ */
/* Compute the coefficients c_k of Φ_209(x) (k = 0 … 180) once.  The
   coefficients are small integers (−1, 0, 1), so we use `int` for the
   temporary array.  They are stored in `coeff[k]`.                     */
/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Periodic table of sequence values (period length 209).  The table
   is filled on the first call to a().                                 */
/* ------------------------------------------------------------------ */

static seq_t periodic_values[209];
static int  coeffs[181];
static int  init_done = 0;    /* 0 = not yet initialised, 1 = done */

/* ------------------------------------------------------------------ */
/* Main function required by the evaluator.                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!init_done) {
        /* Compute polynomial coefficients once. */
        compute_coeffs(coeffs);

        /* Compute the first 209 terms using the recurrence. */
        periodic_values[0] = 1;                /* a(0) = 1 */

        for (idx_t i = 1; i < (idx_t)209; ++i) {
            seq_t sum = 0;
            unsigned int ii = (unsigned int)i;
            for (int k = 1; k <= 180 && (unsigned int)k <= ii; ++k) {
                sum += (seq_t)coeffs[k] * periodic_values[i - (idx_t)k];
            }
            periodic_values[i] = -sum;
        }
        init_done = 1;
    }

    /* Periodic extension: a(n) = a(n mod 209). */
    return periodic_values[(idx_t)(n % 209)];
}
