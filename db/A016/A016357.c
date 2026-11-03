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
 * Validated: full, pass, 49 terms through n = 48 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Divide polynomial by (x^d - 1) (monic divisor) */
static void poly_div_by_xd_minus_1(seq_t **poly, idx_t *deg, idx_t d)
{
    idx_t cur_deg = *deg;
    idx_t new_deg = cur_deg - d;
    seq_t *quot = mem_new(seq_t, (size_t)(new_deg + 1));

    for (idx_t i = cur_deg; ; --i) {
        seq_t q = (*poly)[i];
        quot[i - d] = q;
        (*poly)[i - d] += q;          /* subtract q * (-1) at degree i‑d */
        if (i == d) break;
    }
    mem_free(*poly);
    *poly = quot;
    *deg  = new_deg;
}

/* Multiply polynomial * (x^d - 1) */
static void poly_mul_by_xd_minus_1(seq_t **poly, idx_t *deg, idx_t d)
{
    idx_t cur_deg = *deg;
    idx_t new_deg = cur_deg + d;
    seq_t *prod = mem_new(seq_t, (size_t)(new_deg + 1));
    for (idx_t i = 0; i <= cur_deg; ++i) {
        prod[i]     -= (*poly)[i];
        prod[i + d] += (*poly)[i];
    }
    mem_free(*poly);
    *poly = prod;
    *deg  = new_deg;
}

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
    seq_t *poly = mem_new(seq_t, 1);
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
    mem_free(poly);
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
