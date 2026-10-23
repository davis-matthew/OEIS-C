/*
 * A042346  Numerators of continued fraction convergents to sqrt(700).
 * https://oeis.org/A042346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   recurrence for continued fraction convergents to sqrt(700)
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 17, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Constants for sqrt(700) */
    const idx_t D = 700;
    const idx_t a0 = 26;         /* floor(sqrt(700)) */

    /* Handle the first convergent directly */
    if (n == 0)
        return (seq_t)26;

    /* Continued fraction state variables */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;

    /* Convergent state variables */
    seq_t p_prev    = 1;  /* p[-1] */
    seq_t p_prev_prev = 0;  /* p[-2] */
    seq_t p_curr;
    seq_t q_prev    = 0;  /* q[-1] */
    seq_t q_prev_prev = 1;  /* q[-2] */
    seq_t q_curr;

    for (idx_t i = 0; i <= n; ++i) {
        if (i > 0) {
            /* Compute next partial quotient */
            idx_t m_next = d * a - m;
            idx_t d_next = (D - m_next * m_next) / d;
            idx_t a_next = (a0 + m_next) / d_next;
            m = m_next;
            d = d_next;
            a = a_next;
        }

        /* Convert a to seq_t for multiplication */
        seq_t aa = (seq_t)a;

        /* Compute convergent p[i] = a*p[i-1] + p[i-2] */
        p_curr = aa * p_prev + p_prev_prev;
        /* Compute convergent q[i] = a*q[i-1] + q[i-2] */
        q_curr = aa * q_prev + q_prev_prev;

        /* Shift state for next iteration */
        p_prev_prev = p_prev;
        p_prev = p_curr;
        q_prev_prev = q_prev;
        q_prev = q_curr;
    }

    return p_curr;
}
