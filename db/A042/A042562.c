/*
 * A042562  Numerators of continued fraction convergents to sqrt(810).
 * https://oeis.org/A042562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) is the numerator of the n‑th convergent of the continued
 *            fraction expansion of sqrt(810) (offset 0)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Constants for sqrt(810) */
    const seq_t S   = 810;   /* |S| < 2^32, fits in any integer type */
    const seq_t a0  = 28;    /* floor(sqrt(810)) */

    if (n == 0) {
        return a0;
    }

    /* Recurrence for convergents:
       p_{-1} = 1, p_0 = a0
       p_k   = a_k * p_{k-1} + p_{k-2}
     */
    seq_t p_prev2 = 1;   /* p_{k-2} */
    seq_t p_prev1 = a0;  /* p_{k-1} */

    /* State of the continued fraction algorithm:
       m_0 = 0, d_0 = 1, a_0 = a0
    */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a_k = a0;      /* current coefficient a_k */

    for (idx_t i = 1; i <= n; ++i) {
        /* Next continued‑fraction coefficient a_{k} */
        seq_t m_new = d * a_k - m;
        seq_t d_new = (S - m_new * m_new) / d;
        a_k = (a0 + m_new) / d_new;

        /* Compute next numerator p_k */
        seq_t p_k = a_k * p_prev1 + p_prev2;

        /* Shift the recurrence window */
        p_prev2 = p_prev1;
        p_prev1 = p_k;

        /* Update continued‑fraction state for next iteration */
        m = m_new;
        d = d_new;
    }

    return p_prev1; /* p_n, the required numerator */
}
