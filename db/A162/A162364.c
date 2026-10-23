/*
 * A162364  Number of reduced words of length n in the Weyl group D_22.
 * https://oeis.org/A162364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in * f(22) * ∏_{i=1}^{21} f(2*i) *
 *            where f(m) = (1-x^m)/(1-x) = 1 + x + … + x^{m-1}
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 14, u64 n <= 67, u128 n <= 462,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* degree of the product polynomial = 462 */
#define MAX_DEG 462

/* core sequence function */
seq_t a(idx_t n)
{
    static seq_t cache[MAX_DEG + 1];
    static bool computed = false;

    if (!computed)
    {
        /* lengths of the factors f(m) */
        static const size_t lens[22] = {
            22, /* f(22) */
            2,4,6,8,10,12,14,16,18,20,
            22,24,26,28,30,32,34,36,38,40,42
        };

        seq_t dp[MAX_DEG + 1];
        seq_t next[MAX_DEG + 1];

        /* initialise dp[0] = 1, others 0 */
        for (size_t i = 0; i <= MAX_DEG; ++i) dp[i] = 0;
        dp[0] = 1;

        /* successive convolutions with each factor (1 + ... + x^{L-1}) */
        for (size_t f = 0; f < 22; ++f)
        {
            size_t L = lens[f];

            /* clear next array */
            for (size_t i = 0; i <= MAX_DEG; ++i) next[i] = 0;

            /* compute convolution */
            for (size_t i = 0; i <= MAX_DEG; ++i)
            {
                seq_t sum = 0;
                /* j runs from 0 to L-1, keep j <= i */
                for (size_t j = 0; j < L && j <= i; ++j)
                    sum += dp[i - j];
                next[i] = sum;
            }

            /* copy back to dp for next factor */
            for (size_t i = 0; i <= MAX_DEG; ++i) dp[i] = next[i];
        }

        /* copy final product into cache */
        for (size_t i = 0; i <= MAX_DEG; ++i) cache[i] = dp[i];
        computed = true;
    }

    if (n > (idx_t)MAX_DEG)
        return 0;
    return cache[(size_t)n];
}
