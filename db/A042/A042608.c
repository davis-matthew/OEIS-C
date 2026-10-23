/*
 * A042608  Numerators of continued fraction convergents to sqrt(833).
 * https://oeis.org/A042608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the nth convergent of sqrt(833)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 16, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* constants for sqrt(833) */
    const idx_t D = 833;
    const idx_t a0 = 28;          /* floor(sqrt(833)) */

    /* initial convergent components: p_{-2}=0, p_{-1}=1, q_{-2}=1, q_{-1}=0 */
    seq_t p_prev2 = 0, p_prev1 = 1, p_curr;
    seq_t q_prev2 = 1, q_prev1 = 0, q_curr;

    /* continued fraction state: m0=0, d0=1, a0=floor(sqrt(D)) */
    idx_t m = 0, d = 1, a_k = a0;

    /* compute k = 0 */
    p_curr = (seq_t)a_k * p_prev1 + p_prev2;   /* 28 */
    q_curr = (seq_t)a_k * q_prev1 + q_prev2;   /* 1  */
    if (n == 0)
        return p_curr;

    /* shift to prepare for the next step */
    p_prev2 = p_prev1;
    p_prev1 = p_curr;
    q_prev2 = q_prev1;
    q_prev1 = q_curr;

    /* iterate for k = 1 .. n */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* update continued fraction parameters */
        m = d * a_k - m;
        d = (D - (idx_t)m * (idx_t)m) / d;
        a_k = (a0 + m) / d;

        /* compute next convergent */
        p_curr = (seq_t)a_k * p_prev1 + p_prev2;
        q_curr = (seq_t)a_k * q_prev1 + q_prev2;

        /* shift for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
        q_prev2 = q_prev1;
        q_prev1 = q_curr;

        if (k == n)
            return p_curr;
    }

    /* should never reach here */
    return p_curr;
}
