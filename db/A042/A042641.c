/*
 * A042641  Denominators of continued fraction convergents to sqrt(850).
 * https://oeis.org/A042641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   Denominators of the convergents to sqrt(850). These denominators
 *            follow the recurrence q_{-2}=1, q_{-1}=0, q_n = a_n * q_{n-1} +
 *            q_{n-2} , where a_0 = ⌊sqrt(850)⌋ = 29, and for n≥1 the
 *            coefficients a_n repeat the finite cycle (6, 2, 6, 58) with
 *            period 4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Pre‑compute the periodic continued fraction terms of sqrt(850). */
    static bool period_computed = false;
    static int period[64];
    static int period_len = 0;

    if (!period_computed) {
        const int D = 850;
        const int a0 = 29;
        int m = 0, d = 1, a = a0;
        int k = 0;
        while (1) {
            int m_next = d * a - m;
            int d_next = (D - m_next * m_next) / d;
            int a_next = (a0 + m_next) / d_next;
            period[k++] = a_next;
            m = m_next;
            d = d_next;
            a = a_next;
            if (d_next == 1) break;      /* end of the period */
        }
        period_len = k;
        period_computed = true;
    }

    /* Compute the n-th denominator q_n using the recurrence. */
    seq_t p_prev2 = 0, p_prev1 = 1;
    seq_t q_prev2 = 1, q_prev1 = 0;
    seq_t p = 0, q = 0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t a_coeff;
        if (k == 0) {
            a_coeff = (seq_t)29;             /* a_0 = floor(sqrt(850)) */
        } else {
            int idx = (int)((k - 1) % period_len);
            a_coeff = (seq_t)period[idx];
        }

        p = a_coeff * p_prev1 + p_prev2;
        q = a_coeff * q_prev1 + q_prev2;

        p_prev2 = p_prev1; p_prev1 = p;
        q_prev2 = q_prev1; q_prev1 = q;
    }

    return q;
}
