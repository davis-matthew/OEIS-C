/*
 * A169110  Number of reduced words of length n in Coxeter group on 49
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies a(n) = p(n) + 47·∑_{k=1}^{24} a(n−k) –
 *            1128·a(n−25), where p(0)=p(25)=1, p(n)=2 for 1≤n≤24, and p(n)=0
 *            otherwise
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence starts with a(0)=1. */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    static seq_t buf[25];
    /* reset the ring buffer */
    for (size_t j = 0; j < 25; ++j)
        buf[j] = 0;

    seq_t window_sum = 0;          /* sum of the last 24 terms seen so far */

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t p;
        if (i == 0 || i == 25)
            p = (seq_t)1;
        else if (i < 25)
            p = (seq_t)2;
        else
            p = (seq_t)0;

        seq_t a_val = p + (seq_t)47 * window_sum;
        if (i >= 25)
            a_val -= (seq_t)1128 * buf[(size_t)((i - 25) % 25)];

        buf[(size_t)(i % 25)] = a_val;

        /* update sliding window of the last 24 terms */
        window_sum += a_val;
        if (i >= 24)
            window_sum -= buf[(size_t)((i - 24) % 25)];
    }
    return buf[(size_t)(n % 25)];
}
