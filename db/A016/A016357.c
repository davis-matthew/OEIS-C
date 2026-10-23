/*
 * A016357  210th cyclotomic polynomial.
 * https://oeis.org/A016357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 210th cyclotomic polynomial
 *            Φ_210(x)
 * Range:     i8 n <= 48, i16 n <= 48, i32 n <= 48, i64 n <= 48, i128 n <= 48,
 *            big any n
 * Helpers:   poly_div_by_xd_minus_1, poly_mul_by_xd_minus_1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/poly_div_by_xd_minus_1.h"
#include "../utils/poly_mul_by_xd_minus_1.h"

/* List of all positive divisors of 210 */
static const idx_t divisors[] = {
    1, 2, 3, 5, 6, 7, 10, 14, 15, 21, 30, 35, 42, 70, 105, 210
};
#define NDIV (sizeof(divisors)/sizeof(divisors[0]))

/* μ(210/d) for each divisor d in the same order as 'divisors' */
static const int mu_vals[] = {
    1,  -1, -1, -1,  1,  -1,  1,  1,  1,  1, -1,  1, -1, -1, -1,  1
};

/* Static storage for the coefficients of Φ_210(x) */
static seq_t coeff[49];          /* φ(210) = 48 → 49 coefficients */
static bool coeff_ready = false;

/* Build the coefficients once */
static void build_phi_210(void)
{
    idx_t deg = 0;
    seq_t *poly = (seq_t *)calloc(1, sizeof(seq_t));
    poly[0] = 1;                   /* initial polynomial = 1 */

    /* Multiply by (x^d - 1) for each divisor with μ(210/d) = +1 */
    for (int i = 0; i < NDIV; ++i) {
        if (mu_vals[i] == 1) {
            poly_mul_by_xd_minus_1(&poly, &deg, (idx_t)divisors[i]);
        }
    }

    /* Divide by (x^d - 1) for each divisor with μ(210/d) = -1 */
    for (int i = 0; i < NDIV; ++i) {
        if (mu_vals[i] == -1) {
            poly_div_by_xd_minus_1(&poly, &deg, (idx_t)divisors[i]);
        }
    }

    /* Copy the final coefficients into the static array */
    for (idx_t k = 0; k <= deg; ++k) {
        coeff[k] = poly[k];
    }
    free(poly);
    coeff_ready = true;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (!coeff_ready) {
        build_phi_210();
    }
    if (n <= 48ULL) {               /* coefficient exists */
        return coeff[n];
    }
    return (seq_t)0;                 /* beyond degree → zero */
}
