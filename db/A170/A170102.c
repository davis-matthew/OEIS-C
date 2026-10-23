/*
 * A170102  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 43*∑_{k=1}^{36} a(n-k) - 946*a(n-37) + c(n) where c(n) =
 *            1 if n==0 or n==37, 2 if 1<=n<=36, 0 otherwise
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, if needed */

seq_t a(idx_t n)
{
    /* buffer of the last 37 values: last[0] = a(n-37), last[36] = a(n-1) */
    seq_t last[37] = {0};
    seq_t sum36 = 0;           /* sum of last[1]..last[36] */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t c;
        if (i == 0 || i == 37)
            c = 1;
        else if (i <= 36)
            c = 2;
        else
            c = 0;

        seq_t a_i = (43 * sum36) - (946 * last[0]) + c;

        /* Shift the buffer to make space for a_i */
        seq_t temp = last[1];
        for (int j = 0; j < 36; ++j)
            last[j] = last[j + 1];
        last[36] = a_i;

        /* Update the rolling sum of the last 36 terms */
        sum36 = sum36 - temp + a_i;

        if (i == n)
            return a_i;
    }
    /* unreachable, but satisfy compiler */
    return 0;
}
