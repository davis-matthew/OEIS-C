/*
 * A011637  60th cyclotomic polynomial.
 * https://oeis.org/A011637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of Φ₆₀(x) at degree n; a(n)=0 for n>16
 * Range:     i8 n <= 16, i16 n <= 16, i32 n <= 16, i64 n <= 16, i128 n <= 16,
 *            big any n
 * Helpers:   divide_by_xk_minus_1, multiply_by_xk_minus_1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/divide_by_xk_minus_1.h"
#include "../utils/multiply_by_xk_minus_1.h"

/* Compute all 17 coefficients of Φ₆₀(x). */
static void compute_coefficients(seq_t *coeff_out)
{
    seq_t poly[81];
    for (size_t i = 0; i < 81; ++i) poly[i] = 0;
    poly[0] = -1;            /* -1 for x⁰ term */
    poly[60] = 1;            /* +1 for x⁶⁰ term */
    size_t deg = 60;

    /* multiply by (x¹⁰-1), (x⁶-1), (x⁴-1) (μ=+1 terms) */
    multiply_by_xk_minus_1(poly, &deg, 10);
    multiply_by_xk_minus_1(poly, &deg, 6);
    multiply_by_xk_minus_1(poly, &deg, 4);

    /* divide by (x³⁰-1), (x²⁰-1), (x¹²-1), (x²-1) (μ=-1 terms) */
    divide_by_xk_minus_1(poly, &deg, 30);
    divide_by_xk_minus_1(poly, &deg, 20);
    divide_by_xk_minus_1(poly, &deg, 12);
    divide_by_xk_minus_1(poly, &deg, 2);

    /* store the resulting 17 coefficients (degree 0 … 16) */
    for (size_t i = 0; i < 17; ++i) coeff_out[i] = poly[i];
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t coeff[17];

    if (!initialized) {
        compute_coefficients(coeff);
        initialized = true;
    }

    if (n < 17) return coeff[n];
    return 0;
}
