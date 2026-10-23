/*
 * A042610  Numerators of continued fraction convergents to sqrt(834).
 * https://oeis.org/A042610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 19, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A042610: Numerators of continued‑fraction convergents to sqrt(834)
 *  a(0) = floor(sqrt(834)) = 28
 *  Recurrence:
 *      p_{-1} = 1,  p_0 = a0
 *      p_k   = a_k * p_{k-1} + p_{k-2}   (k ≥ 1)
 *  where a_k are the partial quotients of sqrt(834):
 *      m_0 = 0,  d_0 = 1,  a0 = floor(sqrt(D))
 *      m_{k+1} = d_k * a_k - m_k
 *      d_{k+1} = (D - m_{k+1}^2) / d_k
 *      a_{k+1} = (a0 + m_{k+1}) / d_{k+1}
 */
seq_t a(idx_t n)
{
    const idx_t D   = 834;
    const idx_t a0  = 28;            /* floor(sqrt(834)) */

    if (n == 0) return (seq_t)a0;

    /* continued‑fraction state */
    idx_t m = 0;                     /* m_0 */
    idx_t d = 1;                     /* d_0 */
    idx_t a = a0;                    /* a_0 */

    /* convergent numerators */
    seq_t p_prev2 = 1;               /* p_{-1} */
    seq_t p_prev1 = (seq_t)a0;       /* p_0 */
    seq_t p_curr  = 0;               /* placeholder for p_k */

    for (idx_t k = 1; k <= n; ++k) {
        /* next partial quotient */
        idx_t m_next = d * a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* compute numerator */
        p_curr = (seq_t)a_next * p_prev1 + p_prev2;

        /* shift for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;

        /* update CF state */
        m = m_next;
        d = d_next;
        a = a_next;
    }
    return p_curr;
}
