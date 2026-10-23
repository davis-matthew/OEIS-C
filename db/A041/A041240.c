/*
 * A041240  Numerators of continued fraction convergents to sqrt(132).
 * https://oeis.org/A041240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n‑th convergent of sqrt(132)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t D  = (seq_t)132;      /* D in the Pell equation x^2 – D y^2 = ±1 */
    const seq_t a0 = (seq_t)11;       /* floor(sqrt(132)) */

    if (n == 0)
        return a0;

    /* p_{-1} and p_0 for the recurrence p_k = a_k p_{k-1} + p_{k-2} */
    seq_t p_minus1 = (seq_t)1;
    seq_t p_current = a0;                         /* p_0 */

    /* continued‑fraction variables */
    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;
    seq_t a_k = a0;                               /* current a_k */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t m_next = d * a_k - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;    /* a_{k+1} */

        seq_t p_next = a_next * p_current + p_minus1;

        /* shift for next iteration */
        p_minus1 = p_current;
        p_current = p_next;

        m = m_next;
        d = d_next;
        a_k = a_next;
    }
    return p_current;
}
