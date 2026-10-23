/*
 * A041816  Numerators of continued fraction convergents to sqrt(429).
 * https://oeis.org/A041816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = Numerator of the nth convergent of the continued fraction
 *            for sqrt(429) * * The continued fraction of sqrt(429) is: [20;
 *            (1,2,2,9,1,12,1,9,2,2,1,40)] repeating. * To obtain a(n) we run
 *            the standard recurrence for convergents: * * p_{-2} = 0, p_{-1}
 *            = 1 * p_k = a_k * p_{k-1} + p_{k-2} * * T...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* constants for sqrt(429) */
    const idx_t D   = 429;
    const idx_t a0  = 20;          /* floor(sqrt(429)) */

    /* Special case: first term */
    if (n == 0) {
        return (seq_t)a0;
    }

    /* Initial state after a0 for the continued fraction algorithm */
    idx_t m = a0;                     /* m1 = 1*a0 - 0 = a0 */
    idx_t d = D - m * m;              /* d1 = (D - m^2) / 1 = D - a0^2 */

    /* Convergent recurrence: p_{-2}=0, p_{-1}=1 will be handled by the initial values */
    seq_t p_prevprev = (seq_t)1;      /* p_{-1} */
    seq_t p_prev     = (seq_t)a0;     /* p_0 */

    /* Iterate from i = 1 up to i = n to compute p_n */
    for (idx_t i = 1; i <= n; ++i) {
        /* Current continued fraction coefficient a_i */
        idx_t ai = (a0 + m) / d;

        /* Update state for next coefficient */
        idx_t m_next = d * ai - m;
        idx_t d_next = (D - m_next * m_next) / d;
        m = m_next;
        d = d_next;

        /* Next numerator: p_i = a_i * p_{i-1} + p_{i-2} */
        seq_t p_next = (seq_t)ai * p_prev + p_prevprev;

        /* Shift for next iteration */
        p_prevprev = p_prev;
        p_prev     = p_next;
    }

    return p_prev;    /* p_n */
}
