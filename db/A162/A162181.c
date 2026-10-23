/*
 * A162181  Number of reduced words of length n in the Weyl group B_43.
 * https://oeis.org/A162181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{43}(1 + x + … + x^{2k-1})
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define MAX_DEG 1849   /* 43², the largest exponent with a non‑zero coefficient */

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t poly[MAX_DEG + 1];
    static seq_t tmp[MAX_DEG + 1];

    if (!initialized) {
        /* initialize all coefficients to zero */
        for (size_t i = 0; i <= MAX_DEG; ++i) poly[i] = (seq_t)0;

        poly[0] = (seq_t)1;          /* start with polynomial 1 */
        size_t deg = 0;              /* current maximum degree */

        for (size_t k = 1; k <= 43; ++k) {
            size_t factor = 2 * k - 1;          /* length of the factor (1 + … + x^{factor}) */
            size_t new_deg = deg + factor;      /* new maximum degree after multiplication */

            seq_t sum = (seq_t)0;
            for (size_t j = 0; j <= new_deg; ++j) {
                if (j <= deg) {
                    sum += poly[j];               /* add the new term entering the window */
                }
                if (j > factor) {
                    sum -= poly[j - factor - 1];  /* remove the term leaving the window */
                }
                tmp[j] = sum;                     /* store the coefficient of x^j */
            }

            /* copy the new coefficients back into poly */
            for (size_t j = 0; j <= new_deg; ++j) {
                poly[j] = tmp[j];
            }
            deg = new_deg;
        }
        initialized = true;
    }

    if ((idx_t)MAX_DEG < n) return (seq_t)0;
    return poly[(size_t)n];
}
