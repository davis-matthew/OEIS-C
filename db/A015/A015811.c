/*
 * A015811  Inverse of 1802nd cyclotomic polynomial.
 * https://oeis.org/A015811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the power‑series expansion of
 *            1/Φ_1802(x), where Φ_1802(x) is the 1802nd cyclotomic
 *            polynomial. The sequence is periodic with period 1802. The
 *            implementation computes the polynomial Φ_1802(x) via its product
 *            representation and then uses a linear recurrence to obt...
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   poly_division, poly_mul_xk_minus_one, prepare_sequence
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/poly_division.h"
#include "../utils/poly_mul_xk_minus_one.h"
#include "../utils/prepare_sequence.h"

/* Public function required by the build.  It returns the nth term of
   the sequence, using periodicity.  Calls may arrive in any order. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    if (cache == NULL) {
        cache = prepare_sequence();
    }
    const size_t PERIOD = 1802;
    return cache[(size_t)(n % PERIOD)];
}
