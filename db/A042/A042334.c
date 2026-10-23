/*
 * A042334  Numerators of continued fraction convergents to sqrt(694).
 * https://oeis.org/A042334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent to sqrt(694)
 * Range:     u8 none, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 75,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* 694 is the quadratic surd being approximated */
    seq_t D = (seq_t)694;
    seq_t a0 = floor_sqrt(D);

    if (n == 0)
        return a0;

    /* Continued‑fraction data */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a_coef = a0;

    /* Convergent recurrence p_{-2}=0, p_{-1}=1 */
    seq_t p_prev2 = 0;
    seq_t p_prev1 = 1;
    seq_t p_k = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        if (k == 0) {
            a_coef = a0;
        } else {
            /* Compute next m, d, a using the standard algorithm */
            seq_t m_next = d * a_coef - m;
            seq_t d_next = D - m_next * m_next;
            d_next = d_next / d;           /* integer division */
            m = m_next;
            d = d_next;
            a_coef = (a0 + m) / d;
        }

        /* Convergent numerator */
        p_k = a_coef * p_prev1 + p_prev2;
        if (k == n)
            return p_k;

        /* Shift for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p_k;
    }

    /* Unreachable but keeps compiler happy */
    return p_k;
}
