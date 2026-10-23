/*
 * A041698  Numerators of continued fraction convergents to sqrt(369).
 * https://oeis.org/A041698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent to sqrt(369)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041698: Numerators of continued fraction convergents to sqrt(369). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case: the first convergent (n = 0). */
    if (n == 0)
        return (seq_t)19;

    /* Continued‑fraction data for sqrt(369) */
    const seq_t a0   = (seq_t)19;
    const seq_t D    = (seq_t)369;

    /* Recurrent computation of convergents:
     * p[-2] = 0, p[-1] = 1
     * p_k   = a_k * p_{k-1} + p_{k-2}     (k ≥ 0)
     *
     * The partial quotients a_k for k ≥ 1 are produced using the
     * standard recurrence for quadratic surds:
     *   m_{k+1} = d_k * a_k - m_k
     *   d_{k+1} = (D - m_{k+1}²) / d_k
     *   a_{k+1} = ⌊ (a0 + m_{k+1}) / d_{k+1} ⌋
     * with initialization m_0 = 0, d_0 = 1.
     */

    /* Initial values for the recurrence */
    seq_t p_prevprev = (seq_t)0;      /* p[-2] */
    seq_t p_prev     = (seq_t)1;      /* p[-1] */
    seq_t p_curr;
    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;
    seq_t a_prev = a0;                /* a0 */

    /* First convergent (k = 0) */
    p_curr = a_prev * p_prev + p_prevprev;   /* = a0 * 1 + 0 = a0 */
    p_prevprev = p_prev;        /* 1   */
    p_prev     = p_curr;        /* 19  */

    /* Iterate for k = 1 … n */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* Compute next partial quotient a_k */
        seq_t m_next = d * a_prev - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_k = (a0 + m_next) / d_next;

        /* Update the m/d recurrences for the next step */
        m = m_next;
        d = d_next;
        a_prev = a_k;

        /* Compute the next convergent numerator */
        p_curr = a_k * p_prev + p_prevprev;

        /* Shift for the next iteration */
        p_prevprev = p_prev;
        p_prev = p_curr;
    }

    return p_prev;
}
