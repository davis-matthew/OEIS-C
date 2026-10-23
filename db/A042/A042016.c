/*
 * A042016  Numerators of continued fraction convergents to sqrt(532).
 * https://oeis.org/A042016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = numerator of the nth convergent to sqrt(532)
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 12, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation via continued‑fraction recurrence for sqrt(532). */
seq_t a(idx_t n) {
    /* D = 532, a0 = floor(sqrt(532)) = 23 */
    const idx_t D   = 532;
    const idx_t a0  = 23;

    if (n == 0u)
        return (seq_t)a0;           /* a(0) = 23 */

    /* Continued‑fraction state variables */
    idx_t m  = 0u;        /* m_0 */
    idx_t d  = 1u;        /* d_0 */
    idx_t a  = a0;        /* a_0 */

    /* Convergent numerators: p_{-2}=0, p_{-1}=1 */
    seq_t p_prev2 = 0;
    seq_t p_prev1 = 1;
    seq_t p_curr;

    /* Compute p_0 (n = 0) */
    p_curr = (seq_t)a * p_prev1 + p_prev2;   /* 23*1 + 0 = 23 */
    p_prev2 = p_prev1;      /* 1 */
    p_prev1 = p_curr;       /* 23 */

    /* Generate subsequent partial quotients and numerators */
    for (idx_t i = 1u; i <= n; ++i) {
        /* Next partial quotient via the standard sqrt algorithm */
        idx_t m_next  = d * a - m;
        idx_t d_next  = (D - m_next * m_next) / d;
        idx_t a_next  = (a0 + m_next) / d_next;

        /* Update continued‑fraction state */
        m = m_next;
        d = d_next;
        a = a_next;

        /* Convergent numerator recurrence: p_n = a_n * p_{n-1} + p_{n-2} */
        p_curr = (seq_t)a * p_prev1 + p_prev2;
        if (i == n)
            return p_curr;

        /* Shift for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }

    /* Unreachable, but keeps the compiler happy */
    return p_curr;
}
