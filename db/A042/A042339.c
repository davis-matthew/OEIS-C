/*
 * A042339  Denominators of continued fraction convergents to sqrt(696).
 * https://oeis.org/A042339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent to sqrt(696)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 23, u64 n <= 47, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Continued fraction of sqrt(696): a0 = 26, period = [2,1,1,1,1,1,2,52]   */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Compute and cache the period coefficients once */
    static int period[200];
    static int period_len = 0;

    if (period_len == 0) {
        const int D  = 696;
        const int a0 = 26;          /* floor(sqrt(696)) */

        int m   = 0;
        int d   = 1;
        int a   = a0;
        int idx = 0;

        while (1) {
            int m_new = d * a - m;
            int d_new = (D - m_new * m_new) / d;
            int a_new = (a0 + m_new) / d_new;   /* floor division */

            period[idx++] = a_new;
            if (a_new == 2 * a0) {               /* period terminator */
                period_len = idx;
                break;
            }
            m = m_new;
            d = d_new;
            a = a_new;
        }
    }

    /* Handle first two terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Recurrence: Q_{-2}=0, Q_{-1}=1, Q0=1, Q1=2, Q_n = a_n*Q_{n-1} + Q_{n-2} */
    seq_t q_prev2 = (seq_t)1;  /* Q0 */
    seq_t q_prev1 = (seq_t)2;  /* Q1 */
    seq_t q_curr  = q_prev1;   /* placeholder */

    for (idx_t i = 2; i <= n; ++i) {
        int coeff = period[(i - 1) % period_len];   /* a_i */
        q_curr = q_prev1 * (seq_t)coeff + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q_curr;
    }
    return q_curr;
}
