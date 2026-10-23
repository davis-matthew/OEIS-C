/*
 * A035770  Coordination sequence for C_33 lattice.
 * https://oeis.org/A035770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(2n)] ((1+x)/(1-x))^33
 * Range:     u8 none, u16 none, u32 none, u64 n <= 11, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t binom33[34];
    /* precompute binomial coefficients binom(33,i) */
    for (idx_t i = 0; i <= 33; ++i)
    {
        if (i == 0 || i == 33)
            binom33[i] = 1;
        else
        {
            seq_t res = 1;
            idx_t k = i;
            if (k > 33 - k) k = 33 - k;
            for (idx_t j = 1; j <= k; ++j)
            {
                res = res * (seq_t)(33 - j + 1) / (seq_t)j;
            }
            binom33[i] = res;
        }
    }

    /* compute the coefficient sum */
    for (idx_t i = 0; i <= 33 && i <= 2 * n; ++i)
    {
        /* m = 32 + 2n - i */
        idx_t m = 32 + (2 * n) - i;
        /* binom(m,32) */
        seq_t res = 1;
        for (idx_t j = 1; j <= 32; ++j)
        {
            res = res * (seq_t)(m - j + 1) / (seq_t)j;
        }
        result += binom33[i] * res;
    }

    return result;
}
