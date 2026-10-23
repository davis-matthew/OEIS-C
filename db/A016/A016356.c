/*
 * A016356  209th cyclotomic polynomial.
 * https://oeis.org/A016356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 209th cyclotomic polynomial
 *            Φ_209(x)
 * Range:     i8 n <= 180, i16 n <= 180, i32 n <= 180, i64 n <= 180,
 *            i128 n <= 180, big any n
 * Helpers:   compute_209_coeffs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/compute_209_coeffs.h"

/* forward declaration of helper to compute the coefficients only once */
static void compute_209_coeffs(seq_t coeff[181]);

seq_t a(idx_t n) {
    static bool initialized = false;
    static seq_t coeff[181];

    if (!initialized) {
        compute_209_coeffs(coeff);
        initialized = true;
    }

    if (n < 181) {
        return coeff[n];
    } else {
        /* Outside the polynomial degree all coefficients are zero */
        return (seq_t)0;
    }
}
