/*
 * A041197  Denominators of continued fraction convergents to sqrt(109).
 * https://oeis.org/A041197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 21, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominator of n-th convergent of sqrt(109) continued fraction */
seq_t a(idx_t n)
{
    const seq_t D  = 109;     /* The radicand */
    const seq_t a0 = 10;      /* floor(sqrt(109)) */

    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;

    seq_t P_prev2 = 0;
    seq_t Q_prev2 = 1;
    seq_t P_prev1 = 1;
    seq_t Q_prev1 = 0;
    seq_t P, Q;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t ai = (i == 0) ? a0 : a;
        P = ai * P_prev1 + P_prev2;
        Q = ai * Q_prev1 + Q_prev2;
        if (i == n) return Q;

        /* Update m, d, a for next partial quotient */
        seq_t m_next = d * ai - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a = a_next;

        /* Shift the convergent recurrences */
        P_prev2 = P_prev1;
        Q_prev2 = Q_prev1;
        P_prev1 = P;
        Q_prev1 = Q;
    }
    return 0; /* should never reach here */
}
