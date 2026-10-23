/*
 * A161716  Number of reduced words of length n in the Weyl group B_7.
 * https://oeis.org/A161716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{7} (1 - x^{2k})/(1 - x)
 * Range:     u8 n <= 4, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Compute the coefficients of the generating polynomial once. */
static bool coeff_initialized = false;
static seq_t coeff[50];
static size_t coeff_len = 0;

static void compute_coeffs(void)
{
    if (coeff_initialized)
        return;
    /* initialise to zero and set the constant term */
    for (size_t i = 0; i < 50; ++i)
        coeff[i] = 0;
    coeff[0] = 1;
    coeff_len = 1;

    /* multiply by each factor (1 + x + ... + x^(2k-1)) for k = 1..7 */
    for (size_t k = 1; k <= 7; ++k)
    {
        size_t add_len   = 2 * k;          /* number of terms in this factor */
        size_t new_len   = coeff_len + add_len - 1; /* new polynomial length */

        seq_t temp[50];
        for (size_t i = 0; i < 50; ++i)
            temp[i] = 0;

        for (size_t i = 0; i < coeff_len; ++i)
            for (size_t j = 0; j < add_len; ++j)
                temp[i + j] += coeff[i];

        for (size_t i = 0; i < new_len; ++i)
            coeff[i] = temp[i];

        coeff_len = new_len;
    }

    coeff_initialized = true;
}

seq_t a(idx_t n)
{
    compute_coeffs();

    if (n >= coeff_len)
        return 0; /* coefficient is zero for n > 49 */

    return coeff[(size_t)n];
}
