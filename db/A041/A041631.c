/*
 * A041631  Denominators of continued fraction convergents to sqrt(334).
 * https://oeis.org/A041631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of nth convergent to sqrt(334)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 18, u64 n <= 40, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Returns the n‑th denominator of the continued‑fraction convergents of sqrt(334).
   The sequence starts with a(0)=1, a(1)=3, a(2)=4, ... */
seq_t a(idx_t n)
{
    /* D is the non‑square integer whose square root we are approximating.  */
    const uint64_t D  = 334;
    /* a0 is the integer part of sqrt(D).  */
    const uint64_t A0 = 18;   /* floor(sqrt(334)) = 18 */

    /* Handle the first convergent separately.  */
    if (n == 0)
        return (seq_t)1;      /* denominator q0 = 1 */

    /* Continued‑fraction state variables (all fit in 64 bit for D=334). */
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a_prev = A0;   /* a0 for the first step of the algorithm */

    /* Convergent recurrence:  q_i = a_i * q_{i-1} + q_{i-2}  */
    seq_t p_prevprev = 0, p_prev = 1;   /* p_{-2}=0, p_{-1}=1   */
    seq_t q_prevprev = 1, q_prev = 0;   /* q_{-2}=1, q_{-1}=0   */
    seq_t p_cur, q_cur;

    /* Compute convergent for i = 0 (use a0). */
    p_cur = (seq_t)A0 * p_prev + p_prevprev;
    q_cur = (seq_t)A0 * q_prev + q_prevprev;

    /* Shift state for the next iteration. */
    p_prevprev = p_prev;
    p_prev = p_cur;
    q_prevprev = q_prev;
    q_prev = q_cur;

    /* Iterate from i = 1 to n.  For each i, compute the next CF coefficient
       a_i and update the convergents.  */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* Compute the next terms of the continued fraction. */
        uint64_t m_next = d * a_prev - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_cur  = (A0 + m_next) / d_next;

        /* Update convergents using the new coefficient a_cur. */
        p_cur = (seq_t)a_cur * p_prev + p_prevprev;
        q_cur = (seq_t)a_cur * q_prev + q_prevprev;

        /* If we have reached the requested index, return the denominator. */
        if (i == n)
            return q_cur;

        /* Prepare for the next step. */
        p_prevprev = p_prev;
        p_prev = p_cur;
        q_prevprev = q_prev;
        q_prev = q_cur;

        a_prev = a_cur;
        m = m_next;
        d = d_next;
    }

    /* Should never be reached.  */
    return (seq_t)0;
}
