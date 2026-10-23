/*
 * A161976  Number of reduced words of length n in the Weyl group B_30.
 * https://oeis.org/A161976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1..30} (1 - x^(2k))/(1 - x)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 40, u128 n <= 343,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The product expands to a polynomial of degree 900, so a(n)=0 for n>900. */

#include <stdbool.h>
#include <stddef.h>

#define MAX_DEG 900

seq_t a(idx_t n)
{
    static seq_t coeff[MAX_DEG + 1];
    static bool built = false;

    if (!built) {
        /* initialise all coefficients to 0 */
        for (size_t i = 0; i <= MAX_DEG; ++i)
            coeff[i] = 0;

        /* start with the empty product, i.e. coefficient of x^0 is 1 */
        coeff[0] = 1;
        size_t cur_deg = 0;            /* current maximum degree of coeff */

        for (size_t k = 1; k <= 30; ++k) {
            size_t len = 2 * k;         /* length of the factor polynomial
                                            (1 + x + … + x^(2k-1)) */
            seq_t temp[MAX_DEG + 1];
            for (size_t i = 0; i <= MAX_DEG; ++i)
                temp[i] = 0;

            /* convolution with the current factor */
            for (size_t i = 0; i <= cur_deg; ++i) {
                seq_t val = coeff[i];
                if (val == 0)
                    continue;
                for (size_t j = 0; j < len; ++j)
                    temp[i + j] += val;
            }

            cur_deg += len - 1;
            if (cur_deg > MAX_DEG)
                cur_deg = MAX_DEG;      /* safety, though not expected */

            for (size_t i = 0; i <= cur_deg; ++i)
                coeff[i] = temp[i];
        }
        built = true;
    }

    if (n <= (idx_t)MAX_DEG)
        return coeff[(size_t)n];
    else
        return (seq_t)0;
}
