/*
 * A041514  Numerators of continued fraction convergents to sqrt(274).
 * https://oeis.org/A041514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of the convergents to sqrt(274);
   recurrence: p_{-2}=0, p_{-1}=1, p_k = a_k p_{k-1} + p_{k-2},
   where a_k are the partial quotients of sqrt(274) with a_0=16. */

seq_t a(idx_t n)
{
    /* partial quotient state */
    idx_t const a0 = 16;      /* floor(sqrt(274)) */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;

    /* previous two numerators */
    seq_t p_prevprev = (seq_t)0;
    seq_t p_prev     = (seq_t)1;

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t ak    = (seq_t)a;
        seq_t p_cur = ak * p_prev + p_prevprev;

        if (k == n)
            return p_cur;

        /* shift numerators for next step */
        p_prevprev = p_prev;
        p_prev     = p_cur;

        /* compute next partial quotient a_{k+1} */
        idx_t m_next = d * a - m;
        idx_t d_next = (274 - (m_next * m_next)) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a = a_next;
    }

    /* control should never reach here */
    return (seq_t)0;
}
