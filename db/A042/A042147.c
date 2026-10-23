/*
 * A042147  Denominators of continued fraction convergents to sqrt(598).
 * https://oeis.org/A042147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 15, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042147: denominators of continued‑fraction convergents to sqrt(598). */
/* The recurrence uses the partial quotients of the continued fraction of sqrt(598). */
seq_t a(idx_t n)
{
    const idx_t D  = 598;
    const idx_t a0 = 24;                 /* floor(sqrt(598)) */

    /* q_-2 = 1, q_-1 = 0 for the standard recurrence q_n = a_n*q_{n-1} + q_{n-2} */
    seq_t q_prevprev = (seq_t)1;         /* q_{i-2} */
    seq_t q_prev     = (seq_t)0;         /* q_{i-1} */
    seq_t q_cur;                        /* q_i */

    /* state for generating partial quotients a_i */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;                       /* a_0 */

    for (idx_t i = 0; i <= n; ++i)
    {
        /* compute the i-th denominator */
        q_cur = (seq_t)a * q_prev + q_prevprev;

        if (i == n)
            break;                      /* finished – return the requested term */

        /* advance to the next partial quotient a_{i+1} */
        idx_t m_next = d * a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* shift denominators for next iteration */
        q_prevprev = q_prev;
        q_prev     = q_cur;

        /* update state */
        m = m_next;
        d = d_next;
        a = a_next;
    }

    return q_cur;
}
