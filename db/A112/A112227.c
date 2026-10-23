/*
 * A112227  A scaled Hermite triangle.
 * https://oeis.org/A112227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = (coeff of x^k in H_r(x)) / 2^k * where H_0 = 1, H_1 = 2x,
 *            * H_n = 2x H_{n-1} - 2(n-1) H_{n-2}, * and the pair (r,k)
 *            satisfies r(r+1)/2 ≤ n < (r+1)(r+2)/2 and * k = n - r(r+1)/2.
 *            The triangle T(r,k) = coeff / 2^k forms A112227
 * Range:     i8 n <= 17, i16 n <= 47, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Helpers:   integer_sqrt, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/integer_sqrt.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* --- Determine row r and column k for the requested entry --- */
    idx_t r, k;
    {
        /* Compute integer sqrt of 8*n+1 safely for the expected limits. */
        idx_t disc = (idx_t)8 * n + 1;
        idx_t root = integer_sqrt(disc);           /* floor(sqrt(disc)) */
        r = (root - 1) / 2;                        /* floor ((sqrt(8n+1)-1)/2) */

        /* Correct r in case of rounding errors (double-check bounds). */
        while ((r + 1) * (r + 2) / 2 <= n) ++r;
        while (r * (r + 1) / 2 > n) --r;

        k = n - r * (r + 1) / 2;
    }

    /* --- Static storage for previously computed coefficients --- */
    static seq_t *coeff     = NULL;   /* linear storage for C(n,k) */
    static size_t coeff_cap = 0;      /* allocated size in elements   */
    static idx_t   max_r     = 0;     /* highest n computed so far    */

    /* Needed capacity for rows up to r. */
    size_t needed_cap = (size_t)(r + 1) * (r + 2) / 2;

    /* Allocate or grow array as required. */
    if (!coeff) {           /* first call */
        coeff_cap = needed_cap;
        coeff = (seq_t *)malloc(coeff_cap * sizeof(seq_t));
        if (!coeff) return 0;           /* allocation failure – unlikely */
        coeff[0] = (seq_t)1;            /* C(0,0) */
        max_r = 0;
    } else if (coeff_cap < needed_cap) {
        coeff_cap = needed_cap;
        seq_t *tmp = (seq_t *)realloc(coeff, coeff_cap * sizeof(seq_t));
        if (!tmp) return 0;             /* allocation failure */
        coeff = tmp;
    }

    /* --- Compute any missing rows up to r. --- */
    for (idx_t rr = max_r + 1; rr <= r; ++rr) {
        size_t start_rr  = (size_t)rr * (rr + 1) / 2;
        size_t start_rr1 = (size_t)(rr - 1) * rr / 2;   /* start of row rs-1 */
        size_t start_rr2 = 0;
        if (rr >= 2) start_rr2 = (size_t)(rr - 2) * (rr - 1) / 2;

        for (idx_t kk = 0; kk <= rr; ++kk) {
            seq_t left  = 0;
            seq_t right = 0;

            if (kk > 0) {
                /* 2 * C(rr-1, kk-1) */
                left = coeff[start_rr1 + (kk - 1)] * (seq_t)2;
            }

            if (rr >= 2 && kk <= rr - 2) {
                /* 2*(rr-1) * C(rr-2, kk) */
                right = coeff[start_rr2 + kk] *
                        (seq_t)(2 * (rr - 1));
            }

            coeff[start_rr + kk] = left - right;
        }
        max_r = rr;
    }

    /* --- Retrieve the coefficient and divide by 2^k --- */
    size_t index = (size_t)r * (r + 1) / 2 + k;
    seq_t denom  = pow2(k);                /* 2^k as seq_t */
    seq_t result = coeff[index] / denom;  /* exact division */
    return result;
}
