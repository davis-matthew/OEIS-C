/*
 * A162346  Number of reduced words of length n in the Weyl group D_18.
 * https://oeis.org/A162346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 18, u64 n <= 306, u128 n <= 306,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A162346
   Formula: a(n) = coefficient of x^n in
                f(18) * ∏_{i=1}^{17} f(2i),
               where f(m) = (1 - x^m)/(1 - x) = 1 + x + … + x^{m-1}   */

#include <stddef.h>   /* for size_t */
#include <stdbool.h>  /* for bool */

static seq_t coeff[307];   /* coefficients of the polynomial, 0 … 306 */
static bool coeff_ready = false;   /* flag indicating that coeff[] has been filled */

/* Compute the polynomial coefficients once.  */
static void init_coeff(void)
{
    if (coeff_ready)
        return;

    /* Lengths of the factors f(18) and f(2i) for i = 1 … 17.  */
    static const int fs[18] = {
        18, 2, 4, 6, 8, 10, 12, 14, 16, 18,
        20, 22, 24, 26, 28, 30, 32, 34
    };

    /* Temporary storage for convolution.  */
    static seq_t tmp[307];

    /* initialise polynomial to 1 (coeff[0] = 1).  */
    for (size_t i = 0; i < 307; ++i)
        coeff[i] = 0;
    coeff[0] = (seq_t)1;

    size_t cur_deg = 0;   /* current degree of the polynomial */

    for (int k = 0; k < 18; ++k) {
        int m = fs[k];                       /* length of current factor f(m) */
        size_t new_deg = cur_deg + (size_t)(m - 1);

        /* zero temporary array up to new_deg   */
        for (size_t i = 0; i <= new_deg; ++i)
            tmp[i] = 0;

        /* convolution of current polynomial with (1 + x + … + x^{m-1}) */
        for (size_t i = 0; i <= cur_deg; ++i) {
            seq_t vi = coeff[i];
            for (int j = 0; j < m; ++j)
                tmp[i + j] += vi;
        }

        /* copy result back into coeff[]
           only indices up to new_deg are needed, higher ones stay zero   */
        for (size_t i = 0; i <= new_deg; ++i)
            coeff[i] = tmp[i];

        cur_deg = new_deg;
    }

    coeff_ready = true;
}

/* Public function required by the test harness.  */
seq_t a(idx_t n)
{
    /* Values are known only up to degree 306; beyond that they are zero.  */
    if (n > 306u)
        return (seq_t)0;

    init_coeff();
    return coeff[(size_t)n];
}
