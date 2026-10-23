/*
 * A161859  Number of reduced words of length n in the Weyl group B_13.
 * https://oeis.org/A161859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 31, u64 n <= 169, u128 n <= 169,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:  A161859
   a(n) = coefficient of x^n in ∏_{k=1..13} (1 + x + … + x^(2k-1))
   (the product for the Weyl group B_13) */

#include <stdbool.h>          /* for bool, true, false */

#define MAX_DEG 169           /* maximum degree of the product polynomial */

/* static arrays for the coefficients and a temporary buffer */
static seq_t coeff[MAX_DEG + 1];
static bool coeff_initialized = false;

/* compute the coefficients once */
static void compute_coeff(void)
{
    /* zero the coefficient array */
    for (idx_t i = 0; i <= MAX_DEG; ++i)
        coeff[i] = 0;
    coeff[0] = 1;

    idx_t cur_max = 0;            /* current maximum degree with non‑zero coeff */

    for (idx_t k = 1; k <= 13; ++k) {
        idx_t poly_deg = 2 * k - 1;         /* degree of the k‑th factor */
        idx_t new_max = cur_max + poly_deg;

        /* temporary array to accumulate the new coefficients */
        static seq_t tmp[MAX_DEG + 1];
        for (idx_t i = 0; i <= new_max; ++i)
            tmp[i] = 0;

        for (idx_t i = 0; i <= cur_max; ++i) {
            seq_t val = coeff[i];
            if (val == 0) continue;
            for (idx_t j = 0; j <= poly_deg; ++j) {
                tmp[i + j] += val;
            }
        }

        /* copy back to the main array */
        for (idx_t i = 0; i <= new_max; ++i)
            coeff[i] = tmp[i];

        cur_max = new_max;
    }

    coeff_initialized = true;
}

seq_t a(idx_t n)
{
    if (!coeff_initialized)
        compute_coeff();

    if (n > MAX_DEG)
        return 0;
    return coeff[n];
}
