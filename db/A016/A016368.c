/*
 * A016368  260th cyclotomic polynomial.
 * https://oeis.org/A016368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 96, i16 n <= 96, i32 n <= 96, i64 n <= 96, i128 n <= 96,
 *            big any n
 * Validated: full, pass, 97 terms through n = 96 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the coefficient of x^n in the 260th cyclotomic polynomial Φ_260(x).
 * The polynomial is computed once by multiplying/dividing by factors (x^d-1) raised to
 * the Möbius μ(260/d) for each divisor d of 260.
 */
#include <stddef.h>
#include <stdint.h>

/* divide polynomial coeff[] by (x^d - 1) */
static void poly_div_shift(seq_t *coeff, size_t *deg, size_t d)
{
    size_t old_deg = *deg;
    size_t new_deg = old_deg - d;
    seq_t q[305];

    for (size_t k = 0; k <= new_deg; ++k) {
        if (k < d)
            q[k] = -coeff[k];
        else
            q[k] = q[k - d] - coeff[k];
    }

    for (size_t i = 0; i <= new_deg; ++i) coeff[i] = q[i];
    *deg = new_deg;
}

/* multiply polynomial coeff[] by (x^d - 1) */
static void poly_mul_shift(seq_t *coeff, size_t *deg, size_t d)
{
    size_t new_deg = *deg + d;
    seq_t tmp[305]; /* 305 > maximum possible degree (300) */
    for (size_t i = 0; i <= new_deg; ++i) tmp[i] = 0;

    for (size_t i = 0; i <= *deg; ++i) {
        tmp[i] += coeff[i];        /* copy the -1 part */
        tmp[i + d] -= coeff[i];    /* shift and subtract */
    }

    for (size_t i = 0; i <= new_deg; ++i) coeff[i] = tmp[i];
    *deg = new_deg;
}

seq_t a(idx_t n)
{
    static int initialized = 0;
    static seq_t coeff[305];
    static size_t deg = 0;

    if (!initialized) {
        /* Start from polynomial 1 */
        coeff[0] = 1;
        deg = 0;

        /* Factors where μ(260/d) = +1 (positive exponents) */
        const size_t pos_divs[4] = {4, 10, 26, 260};
        const size_t pos_cnt = 4;
        for (size_t i = 0; i < pos_cnt; ++i)
            poly_mul_shift(coeff, &deg, pos_divs[i]);

        /* Factors where μ(260/d) = -1 (negative exponents) */
        const size_t neg_divs[4] = {2, 20, 52, 130};
        const size_t neg_cnt = 4;
        for (size_t i = 0; i < neg_cnt; ++i)
            poly_div_shift(coeff, &deg, neg_divs[i]);

        initialized = 1;
    }

    if ((idx_t)deg >= n)
        return coeff[(size_t)n];
    else
        return 0;
}
