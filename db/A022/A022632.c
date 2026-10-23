/*
 * A022632  Expansion of Product_{m>=1} (1 + m*q^m)^4.
 * https://oeis.org/A022632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in ∏_{m≥1} (1 + m*q^m)^4
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 27, u64 n <= 73, u128 n <= 202,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation expands the product by dynamic programming.  */
/* Complexity is O(n^2) in the number of terms, which is acceptable   */
/* for n up to about 10000.                                          */

#include <stdlib.h>
#include <stdint.h>

/* you may add any needed includes, but keep the code compliant to C11/C++17 */

seq_t a(idx_t n)
{
        /* handle the trivial case */
        if (n == 0)
                return (seq_t)1;

        /* allocate coefficient array of size n+1, initialise to zero */
        seq_t *coef = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!coef)
                return (seq_t)0;            /* memory allocation failed */
        coef[0] = (seq_t)1;

        /* temporary array to hold the coefficients before each m‑step   */
        seq_t *tmp  = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) {
                free(coef);
                return (seq_t)0;
        }

        /* binomial coefficients for (1 + x)^4 */
        static const seq_t C4[5] = {
                (seq_t)1, (seq_t)4, (seq_t)6, (seq_t)4, (seq_t)1
        };

        /* main loop over m = 1 .. n */
        for (idx_t m = 1; m <= n; ++m) {
                /* copy current coefficients */
                for (size_t i = 0; i <= (size_t)n; ++i)
                        tmp[i] = coef[i];

                seq_t m_pow = (seq_t)1;          /* m^k */
                /* k = 1 .. 4 (k = 0 is handled automatically by the
                   unchanged term coef[i] ) */
                for (int k = 1; k <= 4; ++k) {
                        m_pow *= (seq_t)m;            /* m^k */
                        seq_t factor = C4[k] * m_pow; /* C(4,k)*m^k */
                        /* exponent offset for this k */
                        idx_t shift = m * (idx_t)k;
                        for (size_t i = 0; i + shift <= (size_t)n; ++i)
                                coef[i + shift] += tmp[i] * factor;
                }
        }

        /* result */
        seq_t result = coef[n];

        /* free memory */
        free(tmp);
        free(coef);

        return result;
}
