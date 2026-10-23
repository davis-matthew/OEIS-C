/*
 * A161694  Number of reduced words of length n in the Weyl group A_49.
 * https://oeis.org/A161694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{49} (1 + x + … + x^k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The product expands to a polynomial whose degree is 49·50/2 = 1225.
    For n > 1225 the coefficient is 0. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Degree of the polynomial for m = 49 */
    enum { MAX_DEGREE = 49 * 50 / 2 };          /* 1225 */
    static seq_t coeff[MAX_DEGREE + 1];
    static bool ready = false;

    if (!ready)
    {
        /* initialise coefficient array */
        for (int i = 0; i <= MAX_DEGREE; ++i)
            coeff[i] = 0;
        coeff[0] = 1;                     /* start with the constant polynomial 1 */

        int current_max = 0;              /* highest exponent presently represented */
        static seq_t temp[MAX_DEGREE + 1];

        /* successive convolutions with (1 + x + … + x^k) for k = 1..49 */
        for (int k = 1; k <= 49; ++k)
        {
            int new_max = current_max + k;

            /* clear temporary array up to new_max */
            for (int i = 0; i <= new_max; ++i)
                temp[i] = 0;

            /* convolution: for each existing term coeff[j] add it to
               temp[j + i] for i = 0..k                           */
            for (int j = 0; j <= current_max; ++j)
            {
                seq_t val = coeff[j];
                for (int i = 0; i <= k; ++i)
                    temp[j + i] += val;
            }

            /* copy result back into coeff array */
            for (int i = 0; i <= new_max; ++i)
                coeff[i] = temp[i];

            current_max = new_max;
        }

        ready = true;
    }

    if (n <= (idx_t)MAX_DEGREE)
        return coeff[(int)n];
    else
        return 0;   /* for n > 1225 all coefficients are zero */
}
