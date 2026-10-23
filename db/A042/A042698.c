/*
 * A042698  Numerators of continued fraction convergents to sqrt(879).
 * https://oeis.org/A042698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent of sqrt(879) Recurrence:
 *            p_{-2}=0, p_{-1}=1, a_0=29. For k >= 0, m_{k+1} = d_k*a_k - m_k
 *            d_{k+1} = (879 - m_{k+1}^2) / d_k a_{k+1} = floor((a_0 +
 *            m_{k+1}) / d_{k+1}) Numerator: p_k = a_k * p_{k-1} + p_{k-2}
 *            Return a(n)=p_n
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 17, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t A0 = 29;
    const seq_t D  = 879;

    seq_t m = 0, d = 1, a_cur = A0;
    seq_t p_prev2 = 0, p_prev1 = 1, p = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        /* Compute the numerator of the k-th convergent. */
        p = a_cur * p_prev1 + p_prev2;
        if (k == n) return p;

        /* Advance to the next partial quotient of the continued fraction. */
        seq_t m_next = d * a_cur - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (A0 + m_next) / d_next;

        /* Update recurrence state. */
        p_prev2 = p_prev1;
        p_prev1 = p;
        m = m_next;
        d = d_next;
        a_cur = a_next;
    }

    /* Unreachable, but required to satisfy every code path. */
    return 0;
}
