/*
 * A042827  Denominators of continued fraction convergents to sqrt(944).
 * https://oeis.org/A042827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 28, u64 n <= 53, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of convergents to sqrt(944) – computed via continued fraction. */
seq_t a(idx_t n)
{
    const idx_t D   = 944;   /* the radicand */
    const idx_t a0  = 30;    /* floor(sqrt(944)) */

    /* current state of the continued‑fraction algorithm */
    idx_t m = 0;             /* previous m value   */
    idx_t d = 1;             /* previous d value   */
    idx_t a_k = a0;          /* current partial quotient */

    /* convergent recurrence:  p_{-2}=0, p_{-1}=1 ;  q_{-2}=1, q_{-1}=0 */
    seq_t p_m2 = 0;          /* p_{-2} */
    seq_t p_m1 = 1;          /* p_{-1} */
    seq_t q_m2 = 1;          /* q_{-2} */
    seq_t q_m1 = 0;          /* q_{-1} */

    for (idx_t k = 0; k <= n; ++k)
    {
        if (k > 0)
        {
            /* next Euclidean step */
            idx_t m_next = d * a_k - m;
            idx_t d_next = (D - m_next * m_next) / d;
            idx_t a_next = (a0 + m_next) / d_next;

            m = m_next;
            d = d_next;
            a_k = a_next;
        }

        /* compute next convergent */
        seq_t p = a_k * p_m1 + p_m2;
        seq_t q = a_k * q_m1 + q_m2;

        if (k == n)
            return q;          /* denominator of the nth convergent */

        /* shift for next iteration */
        p_m2 = p_m1;
        p_m1 = p;
        q_m2 = q_m1;
        q_m1 = q;
    }

    /* unreachable – return 0 to satisfy the compiler */
    return 0;
}
