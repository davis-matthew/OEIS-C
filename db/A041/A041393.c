/*
 * A041393  Denominators of continued fraction convergents to sqrt(211).
 * https://oeis.org/A041393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 22, u64 n <= 44, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A041393 – Denominators of continued‑fraction convergents to sqrt(211)
 *
 *  The denominators q_n of the convergents of sqrt(211) satisfy
 *      q_{-2} = 1,  q_{-1} = 0
 *      q_n   = a_n * q_{n-1} + q_{n-2}  (n ≥ 0)
 *
 *  where the partial quotients a_n are produced by the standard
 *  recurrence for the continued fraction of sqrt(211):
 *      m_0 = 0, d_0 = 1, a_0 = ⌊√211⌋ = 14
 *      for k ≥ 0:
 *          m_{k+1} = d_k * a_k - m_k
 *          d_{k+1} = (211 - m_{k+1}^2) / d_k
 *          a_{k+1} = ⌊(a_0 + m_{k+1}) / d_{k+1}⌋
 *
 *  The implementation below uses only integer arithmetic and
 *  operates in terms of the type seq_t (which can be 8/16/32/64
 *  bits or arbitrary‑precision).  The function works for any n ≥ 0
 *  and for repeated (unordered) calls because it is fully
 *  deterministic and does not rely on any global state.
 */
seq_t a(idx_t n)
{
    /* constants for sqrt(211) */
    const seq_t D   = 211;   /* the radicand */
    const seq_t A0  = 14;    /* floor(sqrt(211)) */

    /* continued‑fraction state (m_k, d_k) */
    seq_t m = 0;
    seq_t d = 1;

    /* convergents state */
    seq_t q_prevprev = 1;    /* q_{-2} */
    seq_t q_prev     = 0;    /* q_{-1} */

    /* current partial quotient a_k */
    seq_t a_k = A0;

    for (idx_t i = 0; ; ++i) {
        /* compute q_i = a_k * q_{i-1} + q_{i-2} */
        seq_t q = a_k * q_prev + q_prevprev;
        if (i == n) return q;

        /* shift convergents for next step */
        q_prevprev = q_prev;
        q_prev     = q;

        /* compute next partial quotient a_{k+1} */
        seq_t m_next = d * a_k - m;                     /* m_{k+1}   */
        seq_t d_next = (D - m_next * m_next) / d;       /* d_{k+1}   */
        seq_t a_next = (A0 + m_next) / d_next;          /* a_{k+1}   */

        /* update state for next iteration */
        m = m_next;
        d = d_next;
        a_k = a_next;
    }
}
