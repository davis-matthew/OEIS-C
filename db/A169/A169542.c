/*
 * A169542  Number of reduced words of length n in Coxeter group on 49
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(n)=c(n)+47*∑_{i=0}^{n-1}a(i) for 1≤n≤33 (c(n)=2,
 *            c(0)=1), a(34)=1-1128*a(0)+47*∑_{i=1}^{33}a(i), for n≥35:
 *            a(n)=47*∑_{k=1}^{33}a(n−k)−1128*a(n−34)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by a linear recurrence of order 34. */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)1;

    /* ring buffer for the last 34 values */
    seq_t buf[34];
    for (size_t i = 0; i < 34U; ++i) buf[i] = (seq_t)0;

    /* a(0) */
    buf[0] = (seq_t)1;

    seq_t a0 = buf[0];        /* keep a(0) for use in a(34) */
    seq_t sum_all = a0;       /* ∑_{i=0}^{k} a(i) for current k */
    seq_t sum_last33 = (seq_t)0;   /* will be set after n==34 */

    seq_t a_cur = a0;         /* value to return if n==0 */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t c;
        if (k == 34U) c = (seq_t)1;
        else if (k <= 33U) c = (seq_t)2;
        else c = (seq_t)0;

        if (k <= 33U)                    /* 1 ≤ k ≤ 33 */
        {
            a_cur = c + (seq_t)47 * sum_all;
            /* store and update running sums */
            buf[(size_t)k % 34U] = a_cur;
            sum_all += a_cur;
        }
        else if (k == 34U)                /* compute a(34) */
        {
            seq_t sum_range = sum_all - a0;          /* ∑_{i=1}^{33}a(i) */
            a_cur = c + (seq_t)47 * sum_range - (seq_t)1128 * a0;
            buf[(size_t)k % 34U] = a_cur;
            sum_all += a_cur;
            sum_last33 = sum_range;                  /* initialise window for n≥35 */
        }
        else                                /* k ≥ 35 */
        {
            /* a(k) = 47 * sum_last33 - 1128 * a(k-34) */
            seq_t a_prev34 = buf[(size_t)(k - 34U) % 34U];
            a_cur = (seq_t)47 * sum_last33 - (seq_t)1128 * a_prev34;

            buf[(size_t)k % 34U] = a_cur;
            sum_all += a_cur;

            /* slide the window of the last 33 terms */
            sum_last33 += a_cur;
            seq_t a_prev33 = buf[(size_t)(k - 33U) % 34U];
            sum_last33 -= a_prev33;
        }
    }

    return a_cur;
}
