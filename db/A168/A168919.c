/*
 * A168919  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^21 = I.
 * https://oeis.org/A168919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, let S(n)=∑_{k=max(0,n-20)}^{n-1} a(k). Then if
 *            n≤20: a(n)=48·S(n)+2; else: a(n)=48·S(n)−1176·a(n−21)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>


seq_t a(idx_t n)
{
    seq_t buf[21] = {0};          /* circular buffer for the last 21 terms   */
    seq_t sum_last20 = 0;         /* sum of the last ≤20 terms (for recurrence) */
    seq_t val = 0;                /* current a(n) value                       */
    idx_t cur;

    for (cur = 0; cur <= n; ++cur)
    {
        if (cur == 0)                     /* a(0) = 1 */
        {
            val = 1;
        }
        else if (cur <= 20)                /* 1 ≤ n ≤ 20  */
        {
            val = 48 * sum_last20 + 2;
        }
        else                               /* n ≥ 21      */
        {
            val = 48 * sum_last20 -
                  1176 * buf[(size_t)((cur - 21) % 21)];
        }

        buf[(size_t)(cur % 21)] = val;     /* store current value in the ring buffer */

        sum_last20 += val;                /* update running sum of the last 20 terms */
        if (cur >= 20)
        {
            sum_last20 -= buf[(size_t)((cur - 20) % 21)];
        }
    }

    return val;   /* a(n) */
}
