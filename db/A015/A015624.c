/*
 * A015624  Inverse of 1615th cyclotomic polynomial.
 * https://oeis.org/A015624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Inverse of 1615th cyclotomic polynomial series.
   a(n) = coefficient of x^n in the power‑series expansion of 1/Φ_1615(x).
   Periodic with period 1615. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Allocate an array of seq_t elements, all initialized to zero. */

/* Free an array allocated by alloc_seq_array. */

enum { P = 1615 };

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    /* Static cache for one full period of the sequence. */
    static int initialized = 0;
    static seq_t cache[P];

    if (!initialized)
    {
        /* ----------------------------------------------------------------
           Build the cyclotomic polynomial Φ₁₆₁₅(x) = Π_{d|1615} (x^d – 1)^{μ(1615/d)}.
           For 1615 = 5·17·19, the positive μ values are for d = 5, 17, 19, 1615,
           and the negative μ values are for d = 1, 85, 95, 323.
        --------------------------------------------------------------------*/
        const size_t pos[] = {5, 17, 19, 1615};
        const size_t neg[] = {1, 85, 95, 323};
        const size_t pos_cnt = sizeof(pos) / sizeof(pos[0]);
        const size_t neg_cnt = sizeof(neg) / sizeof(neg[0]);

        /* Start with the constant polynomial 1. */
        size_t deg = 0;
        seq_t *poly = alloc_seq_array(1);
        poly[0] = 1;

        /* Multiply by each (x^d – 1) with μ = +1. */
        for (size_t pi = 0; pi < pos_cnt; ++pi)
        {
            size_t d = pos[pi];
            size_t new_deg = deg + d;
            seq_t *new_poly = alloc_seq_array(new_deg + 1);
            for (size_t i = 0; i <= deg; ++i)
            {
                /* Shift by d and subtract the constant term. */
                new_poly[i + d] += poly[i];
                new_poly[i]       -= poly[i];
            }
            free_seq_array(poly);
            poly = new_poly;
            deg = new_deg;
        }

        /* Divide by each (x^d – 1) with μ = –1. */
        for (size_t ni = 0; ni < neg_cnt; ++ni)
        {
            size_t d = neg[ni];
            size_t m = deg;                   /* current degree of poly */
            size_t new_deg = m - d;
            seq_t *quot = alloc_seq_array(new_deg + 1);

            /* Synthetic division by (x^d – 1). */
            size_t i = m;
            while (i >= d)
            {
                seq_t coeff = poly[i];
                quot[i - d] = coeff;          /* coefficient of the quotient */
                poly[i - d] += coeff;         /* subtract coeff * (–1) → +coeff */
                poly[i] = 0;                  /* leading term eliminated */
                if (i == d) break;
                --i;
            }
            /* After division, the lower part poly[0..d-1] should be zero. */
            free_seq_array(poly);
            poly = quot;
            deg = new_deg;
        }

        /* poly now holds the coefficients of Φ₁₆₁₅(x), degrees 0..deg (deg==1152). */

        /* Compute the inverse power‑series coefficients up to one full period. */
        for (size_t i = 0; i < P; ++i) cache[i] = 0;
        cache[0] = 1;  /* a(0) = 1 */

        for (size_t k = 1; k < P; ++k)
        {
            seq_t sum = 0;
            size_t upper = (k <= deg) ? k : deg;
            for (size_t i = 1; i <= upper; ++i)
            {
                sum += poly[i] * cache[k - i];
            }
            cache[k] = -sum;
        }

        free_seq_array(poly);
        initialized = 1;
    }

    return cache[(size_t)(n % (idx_t)P)];
}
