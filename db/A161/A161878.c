/*
 * A161878  Number of reduced words of length n in the Weyl group B_18.
 * https://oeis.org/A161878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{18} (1 + x + ⋯ + x^{2k-1})
 *            The generating function is a finite polynomial of degree 18² =
 *            324, so a(n) = 0 for n > 324. The coefficients are computed once
 *            and memoised for subsequent calls
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 18, u64 n <= 140, u128 n <= 324,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* maximum degree of the polynomial */
    enum { MAXN = 18 * 18 };          /* 324 */

    /* static storage for memoisation */
    static seq_t coeff[MAXN + 1];
    static seq_t tmp[MAXN + 1];
    static int  initialized = 0;

    if (!initialized) {
        /* initialise coefficient array to zero */
        for (int i = 0; i <= MAXN; ++i)
            coeff[i] = 0;
        coeff[0] = 1;   /* start with polynomial 1 */

        int current_max = 0;   /* highest non‑zero exponent seen so far */

        /* multiply by each factor (1 + x + … + x^{2k-1}) */
        for (int k = 1; k <= 18; ++k) {
            int span = 2 * k - 1;          /* highest exponent in this factor */

            /* zero the temporary array */
            for (int i = 0; i <= MAXN; ++i)
                tmp[i] = 0;

            /* convolution: for each existing exponent e,
               add coeff[e] to all positions e + t (t = 0..span) */
            for (int e = 0; e <= current_max; ++e) {
                seq_t val = coeff[e];
                if (val == 0) continue;
                for (int t = 0; t <= span; ++t)
                    tmp[e + t] += val;
            }

            /* copy back into coeff for next iteration */
            for (int i = 0; i <= MAXN; ++i)
                coeff[i] = tmp[i];

            current_max += span;   /* new maximum exponent */
        }

        initialized = 1;
    }

    /* if the requested index exceeds the polynomial degree, return 0 */
    if (n > (idx_t)MAXN)
        return (seq_t)0;

    return coeff[(size_t)n];
}
