/*
 * A161530  Number of reduced words of length n in the Weyl group A_27.
 * https://oeis.org/A161530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{27} (1 + x + … + x^k)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 48, u128 n <= 378,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>    /* for memcpy, if desired */

seq_t a(idx_t n)
{
    /* table holds coefficients up to degree 378 (27*28/2) */
    static int constructed = 0;
    static seq_t table[379] = {0};

    if (!constructed) {
        seq_t temp[379] = {0};
        seq_t next[379];
        temp[0] = 1;
        size_t current_deg = 0;

        for (int k = 1; k <= 27; ++k) {
            /* zero next up to new degree */
            for (size_t i = 0; i <= current_deg + k; ++i)
                next[i] = 0;

            /* convolution with (1 + x + … + x^k) */
            for (size_t i = 0; i <= current_deg + k; ++i) {
                seq_t sum = 0;
                for (int j = 0; j <= k; ++j) {
                    if (i >= (size_t)j)
                        sum += temp[i - j];
                }
                next[i] = sum;
            }

            /* prepare for next iteration */
            for (size_t i = 0; i <= current_deg + k; ++i)
                temp[i] = next[i];

            current_deg += k;
        }

        /* copy final coefficients to table */
        for (size_t i = 0; i <= 378; ++i)
            table[i] = temp[i];

        constructed = 1;
    }

    if (n < 379)
        return table[n];
    else
        return 0;
}
