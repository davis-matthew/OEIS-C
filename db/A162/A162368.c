/*
 * A162368  Number of reduced words of length n in the Weyl group D_26.
 * https://oeis.org/A162368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in * (1 + x + … + x^25) · ∏_{i=1}^{25}
 *            (1 + x + … + x^{2i-1})
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 49, u128 n <= 650,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t coeff[651];
static bool initialized = false;

/* Compute all coefficients of the growth series for D_26. */
static void init_coeffs(void)
{
    /* Start with polynomial 1. */
    for (size_t i = 0; i <= 650; ++i)
        coeff[i] = 0;
    coeff[0] = 1;
    size_t curr_deg = 0;   /* degree of the current polynomial */

    /* List of factor lengths: 26, then 2,4,…,50. */
    size_t factors[26];
    size_t fk = 0;
    factors[fk++] = 26;
    for (size_t m = 2; m <= 50; m += 2)
        factors[fk++] = m;

    /* Convolve sequentially with each factor. */
    for (size_t f = 0; f < fk; ++f) {
        size_t len = factors[f];          /* length of the factor poly */
        size_t new_deg = curr_deg + len - 1;
        if (new_deg > 650)
            new_deg = 650;

        seq_t tmp[651];
        for (size_t i = 0; i <= new_deg; ++i)
            tmp[i] = 0;

        /* Convolution: (coeff * (1 + x + … + x^{len-1})) */
        for (size_t i = 0; i <= curr_deg; ++i) {
            if (coeff[i] == 0) continue;
            for (size_t j = 0; j < len; ++j) {
                size_t pos = i + j;
                if (pos > new_deg) break;
                tmp[pos] += coeff[i];
            }
        }

        /* Copy back to coeff. */
        for (size_t i = 0; i <= new_deg; ++i)
            coeff[i] = tmp[i];

        curr_deg = new_deg;
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized)
        init_coeffs();

    /* n may be larger than the maximal degree (650),
       in which case the series has coefficient 0. */
    if ((size_t)n > 650)
        return 0;

    return coeff[(size_t)n];
}
