/*
 * A161651  Number of reduced words of length n in the Weyl group A_38.
 * https://oeis.org/A161651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1..38} (1 + x + x^2 + … + x^k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This product is a polynomial of degree 741; coefficients beyond that are 0. */

#include <stdbool.h>
#include <stddef.h>

#define M 38
#define MAX_DEG ((M*(M+1))/2)   /* 741 */

seq_t a(idx_t n)
{
    /* Static arrays hold the polynomial coefficients once computed. */
    static bool initialized = false;
    static seq_t coeff[MAX_DEG + 1];
    static seq_t tmp[MAX_DEG + 1];

    if (!initialized) {
        /* Start with the polynomial 1. */
        for (idx_t i = 0; i <= MAX_DEG; ++i)
            coeff[i] = (seq_t)0;
        coeff[0] = (seq_t)1;

        /* Multiply successively by (1 + x + ... + x^k) for k = 1..M. */
        for (idx_t k = 1; k <= M; ++k) {
            /* Zero temporary array for the next coefficient vector. */
            for (idx_t i = 0; i <= MAX_DEG; ++i)
                tmp[i] = (seq_t)0;

            /* Convolution with a polynomial of ones of degree k. */
            for (idx_t i = 0; i <= MAX_DEG; ++i) {
                /* j runs from 0 to min(k,i). */
                idx_t maxj = (i < k) ? i : k;
                for (idx_t j = 0; j <= maxj; ++j) {
                    tmp[i] += coeff[i - j];
                }
            }

            /* Copy the new coefficients back. */
            for (idx_t i = 0; i <= MAX_DEG; ++i)
                coeff[i] = tmp[i];
        }

        initialized = true;
    }

    if (n <= MAX_DEG)
        return coeff[(idx_t)n];
    else
        return (seq_t)0;
}
