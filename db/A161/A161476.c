/*
 * A161476  Number of reduced words of length n in the Weyl group A_15.
 * https://oeis.org/A161476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{15}(1 + x + … + x^k)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 25, u64 n <= 120, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Array to hold the coefficients of the polynomial */
/* The degree of the polynomial is 120 (=1+2+…+15) */
static seq_t coeff[121];
static bool initialized = false;

/* Initialise the coefficient array once */
static void init_coeff(void)
{
    if (initialized)
        return;

    /* Set all coefficients to zero */
    for (idx_t i = 0; i <= 120; ++i)
        coeff[i] = 0;
    coeff[0] = 1;               /* Empty product contribution */

    idx_t cur_deg = 0;           /* Current maximal degree */

    for (int k = 1; k <= 15; ++k)
    {
        idx_t new_deg = cur_deg + k;
        seq_t tmp[121] = {0};

        for (idx_t i = 0; i <= cur_deg; ++i)
        {
            for (int j = 0; j <= k; ++j)
            {
                tmp[i + j] += coeff[i];
            }
        }

        for (idx_t i = 0; i <= new_deg; ++i)
            coeff[i] = tmp[i];

        cur_deg = new_deg;
    }

    initialized = true;
}

/* Return the n-th term of sequence A161476 */
seq_t a(idx_t n)
{
    init_coeff();

    if (n > 120)
        return 0;

    return coeff[n];
}
