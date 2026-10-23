/*
 * A042095  Denominators of continued fraction convergents to sqrt(571).
 * https://oeis.org/A042095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   Denominators of continued fraction convergents to sqrt(571)
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Constants for sqrt(571) */
    const seq_t D   = (seq_t)571;
    const seq_t a0  = (seq_t)23;   /* floor(sqrt(571)) */

    /* Initial continued fraction state */
    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;
    seq_t a = a0;

    /* Recurrence for denominators:
       q_{-2} = 1, q_{-1} = 0 */
    seq_t q_nm2 = (seq_t)1;     /* q_{-2} */
    seq_t q_nm1 = (seq_t)0;     /* q_{-1} */

    idx_t i;
    for (i = (idx_t)0; i <= n; ++i)
    {
        /* Current coefficient a_i */
        seq_t ai = a;

        /* Compute denominator q_i */
        seq_t q = ai * q_nm1 + q_nm2;
        if (i == n)
            return q;

        /* Shift denominators for next step */
        seq_t temp = q_nm1;
        q_nm1     = q;
        q_nm2     = temp;

        /* Generate next continued fraction term */
        seq_t m_next = d * a - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        /* Update state */
        m = m_next;
        d = d_next;
        a = a_next;
    }
    return (seq_t)0;   /* unreachable, silence compiler */
}
