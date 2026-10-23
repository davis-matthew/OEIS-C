/*
 * A161497  Number of reduced words of length n in the Weyl group A_17.
 * https://oeis.org/A161497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{17} (1 + x + … + x^k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 20, u64 n <= 153, u128 n <= 153,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The degree of the product is 1+2+…+17 = 153. */
    static seq_t coef[154];
    static int initialized = 0;

    if (!initialized)
    {
        /* initial polynomial: 1 */
        for (int i = 0; i < 154; ++i)
            coef[i] = 0;
        coef[0] = 1;
        int cur_deg = 0;

        for (int k = 1; k <= 17; ++k)
        {
            int new_deg = cur_deg + k;
            seq_t tmp[154] = {0};
            for (int i = 0; i <= cur_deg; ++i)
            {
                seq_t c = coef[i];
                for (int j = 0; j <= k; ++j)
                    tmp[i + j] += c;
            }
            for (int i = 0; i <= new_deg; ++i)
                coef[i] = tmp[i];
            cur_deg = new_deg;
        }
        initialized = 1;
    }

    if (n >= 154)
        return 0;
    return coef[(idx_t)n];
}
