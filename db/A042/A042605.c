/*
 * A042605  Denominators of continued fraction convergents to sqrt(831).
 * https://oeis.org/A042605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 26, u64 n <= 50, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042605: Denominators of continued fraction convergents to sqrt(831) */
/* This function computes the n-th denominator of the convergents of sqrt(831). */

seq_t a(idx_t n)
{
    /* Local constants for sqrt(831) */
    const idx_t D = 831;
    const idx_t a0 = 28;          /* floor(sqrt(831)) */

    /* Convergents recurrence variables */
    seq_t p_prev2 = 0, p_prev1 = 1, p_curr = 0;
    seq_t q_prev2 = 1, q_prev1 = 0, q_curr = 0;

    /* Continued fraction state variables */
    idx_t m_prev = 0;
    idx_t d_prev = 1;
    idx_t a_prev = a0;
    idx_t a_cur = a0;

    for (idx_t k = 0; k <= n; ++k) {
        if (k == 0) {
            a_cur = a0;
        } else {
            /* Compute next m, d, and a */
            idx_t m = d_prev * a_prev - m_prev;          /* m_k */
            idx_t d = (D - m * m) / d_prev;             /* d_k */
            a_cur = (a0 + m) / d;                       /* a_k */

            /* Update state for next iteration */
            m_prev = m;
            d_prev = d;
            a_prev = a_cur;
        }

        /* Update convergents */
        p_curr = (seq_t)a_cur * p_prev1 + p_prev2;
        q_curr = (seq_t)a_cur * q_prev1 + q_prev2;

        /* Shift for next step */
        p_prev2 = p_prev1; p_prev1 = p_curr;
        q_prev2 = q_prev1; q_prev1 = q_curr;
    }

    return q_curr;
}
