/*
 * A168906  Number of reduced words of length n in Coxeter group on 37
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^21 = I.
 * https://oeis.org/A168906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1,   a(n)=p_n + 35⋅(a(n−1)+⋯+a(n−20)) − 630⋅a(n−21)
   where      p_n = 1  if n==0 or n==21
              p_n = 2  if 1≤n≤20
              p_n = 0  otherwise */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* sliding window holding the last 21 values:
       last[0] = a(current-1), last[1] = a(current-2), …, last[20] = a(current-21) */
    seq_t last[21] = {0};
    seq_t val = 0;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t p;
        if (i == 0 || i == 21)
            p = (seq_t)1;
        else if (i <= 20)
            p = (seq_t)2;
        else
            p = (seq_t)0;

        /* sum of the previous 20 terms */
        seq_t sum = (seq_t)0;
        for (size_t k = 1; k <= 20; ++k) {
            if (i >= k)
                sum += last[k - 1];
        }

        val = p + (seq_t)35 * sum;
        if (i >= 21)
            val -= (seq_t)630 * last[20];   /* last[20] holds a(i-21) */

        /* shift window for next iteration */
        for (size_t j = 20; j > 0; --j)
            last[j] = last[j - 1];
        last[0] = val;
    }

    return val;
}
