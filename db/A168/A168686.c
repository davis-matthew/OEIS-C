/*
 * A168686  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^17 = I.
 * https://oeis.org/A168686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, and for n ≥ 1 * a(n) = 8*a(n‑1) * – 35*a(n‑17) (if n ≥
 *            17) * + 28*a(n‑18) (if n ≥ 18) * + b(n) * with b(n) = 1 for n =
 *            0 or 1, * –1 for n = 17 or 18, * 0 otherwise. * The sequence
 *            begins a(0)=1, a(1)=9, a(2)=72, … as in the OEIS entry
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* A circular buffer of size 18 holds the last 18 values */
    seq_t buf[18] = {0};
    buf[0] = 1;                     /* a(0) */

    for (idx_t m = 1; m <= n; ++m) {
        int idxPrev = (int)((m - 1) % 18);
        seq_t val = buf[idxPrev] * (seq_t)8;     /* 8 * a(m‑1) */

        if (m >= 17) {
            int idx16 = (int)((m - 17) % 18);
            val -= (seq_t)35 * buf[idx16];
        }
        if (m >= 18) {
            int idx17 = (int)((m - 18) % 18);
            val += (seq_t)28 * buf[idx17];
        }

        /* add the constant term b(m) */
        if (m == 1)      val += (seq_t)1;
        else if (m == 17) val -= (seq_t)1;
        else if (m == 18) val -= (seq_t)1;

        buf[(int)(m % 18)] = val;
    }

    return buf[(int)(n % 18)];
}
