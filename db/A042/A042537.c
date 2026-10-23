/*
 * A042537  Denominators of continued fraction convergents to sqrt(797).
 * https://oeis.org/A042537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n-th convergent of the continued
 *            fraction expansion of sqrt(797)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 29, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Special case for n == 0: first denominator is 1 */
    if (n == 0)
        return (seq_t)1;

    /* Constants for sqrt(797) */
    const idx_t D   = 797;
    const idx_t a0  = 28;   /* floor(sqrt(797)) */

    /* Variables for the continued fraction iteration */
    idx_t m = 0, d = 1, a_term = a0;

    /* Previous two convergent numerators and denominators */
    seq_t p_prev2 = (seq_t)0, p_prev1 = (seq_t)1;
    seq_t q_prev2 = (seq_t)1, q_prev1 = (seq_t)0;
    seq_t p, q;

    for (idx_t k = 0; k <= n; ++k) {
        /* Compute current convergent */
        p = ((seq_t)a_term) * p_prev1 + p_prev2;
        q = ((seq_t)a_term) * q_prev1 + q_prev2;

        if (k == n)
            return q;  /* The n-th denominator */

        /* Generate next term of the continued fraction */
        idx_t m_next = d * a_term - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* Update previous terms for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p;
        q_prev2 = q_prev1;
        q_prev1 = q;
        m = m_next;
        d = d_next;
        a_term = a_next;
    }

    /* Should never reach here */
    return (seq_t)0;
}
