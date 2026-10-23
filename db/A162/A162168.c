/*
 * A162168  Number of reduced words of length n in the Weyl group B_39.
 * https://oeis.org/A162168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1..39} (1 + x + … + x^{2k-1})
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The generating function for B_39:  ∏_{k=1}^{39} (1 − x^{2k})/(1 − x)  */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Constants for the problem */
    static const idx_t M      = 39;
    static const idx_t MAXDEG = M * M;              /* 39² = 1521 */

    /* Cached polynomial and temporary buffer */
    static seq_t *poly = NULL;
    static seq_t *tmp  = NULL;

    /* Current maximum degree of the polynomial built so far */
    static idx_t current_max = 0;

    /* One-time allocation and initialization */
    if (poly == NULL) {
        /* Allocate buffers of size MAXDEG+1 and zero‑initialize them */
        poly = (seq_t *)calloc((size_t)(MAXDEG + 1), sizeof(seq_t));
        tmp  = (seq_t *)calloc((size_t)(MAXDEG + 1), sizeof(seq_t));
        if (poly == NULL || tmp == NULL) {
            /* In an embedded / constraint environment, we could
               handle error here; for this implementation we assume
               allocation succeeds. */
            return (seq_t)0;
        }
        poly[0] = (seq_t)1;   /* start with the constant polynomial 1 */
    }

    /* Build the product if it hasn't been done yet */
    if (current_max == 0) {
        for (idx_t k = 1; k <= M; ++k) {
            /* Length of the factor (1 + x + … + x^{2k-1}) */
            idx_t shift_len = 2 * k - 1;

            /* Clear temporary buffer up to the new degree */
            for (idx_t i = 0; i <= current_max + shift_len; ++i) {
                tmp[i] = (seq_t)0;
            }

            /* Convolution: add shifted copies of the current polynomial */
            for (idx_t s = 0; s <= shift_len; ++s) {
                for (idx_t i = 0; i <= current_max; ++i) {
                    tmp[i + s] += poly[i];
                }
            }

            /* Copy the result back into the main polynomial array */
            for (idx_t i = 0; i <= current_max + shift_len; ++i) {
                poly[i] = tmp[i];
            }

            /* Update the current maximum degree */
            current_max += shift_len;
        }
    }

    /* Return the requested coefficient, or 0 if out of range */
    if (n <= current_max) {
        return poly[n];
    } else {
        return (seq_t)0;
    }
}
