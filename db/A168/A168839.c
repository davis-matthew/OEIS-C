/*
 * A168839  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^20 = I.
 * https://oeis.org/A168839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 + 16 * (a(0)+...+a(n-1)) for 1 ≤ n ≤ 19 a(0) = 1 a(n) =
 *            16 * (a(n-1)+...+a(n-19)) – 136 * a(n-20) for n ≥ 20
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to avoid recomputing values */
    static seq_t *buf = NULL;
    static size_t cap = 0;

    /* Extend cache if needed */
    if ((size_t)n >= cap) {
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – abort */
            exit(1);
        }
        buf = tmp;

        /* Compute new terms */
        for (size_t i = cap; i <= n; ++i) {
            if (i == 0) {
                buf[0] = (seq_t)1;
            } else if (i <= 19) {
                seq_t sum = (seq_t)0;
                for (size_t j = 0; j < i; ++j)
                    sum += buf[j];
                buf[i] = (seq_t)2 + (seq_t)16 * sum;
            } else {
                seq_t sum = (seq_t)0;
                for (size_t j = i - 19; j < i; ++j)
                    sum += buf[j];
                buf[i] = (seq_t)16 * sum - (seq_t)136 * buf[i - 20];
            }
        }
        cap = newcap;
    }

    return buf[(size_t)n];
}
