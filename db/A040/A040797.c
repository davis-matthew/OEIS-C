/*
 * A040797  Continued fraction for sqrt(826).
 * https://oeis.org/A040797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction terms of sqrt(826) a(0) = 28 For n ≥ 1: m₀ =
 *            0, d₀ = 1, a₀ = 28 m_{k+1} = d_k · a_k – m_k d_{k+1} = (826 –
 *            m_{k+1}²) / d_k a_{k+1} = ⌊(28 + m_{k+1}) / d_{k+1}⌋ a(n) = a_n
 * Range:     u8 n <= 1, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t D  = 826;   /* the radicand */
    const seq_t A0 = 28;    /* floor(sqrt(826)) */

    /* Base case */
    if (n == 0)
        return A0;

    /* Variables for the continued‑fraction recurrence */
    seq_t m   = 0;     /* m₀   */
    seq_t d   = 1;     /* d₀   */
    seq_t a_k = A0;    /* a₀   */
    seq_t a_n = 0;     /* placeholder for the final term */

    /* Iterate n times to reach the n‑th term */
    for (idx_t i = 1; i <= n; ++i) {
        /* m_{k+1} = d_k * a_k – m_k */
        seq_t m_next = d * a_k - m;

        /* d_{k+1} = (D – m_{k+1}²) / d_k  (exact division) */
        seq_t d_next = (D - m_next * m_next) / d;

        /* a_{k+1} = floor((A0 + m_{k+1}) / d_{k+1}) */
        a_n = (A0 + m_next) / d_next;

        /* Update state for next iteration */
        m   = m_next;
        d   = d_next;
        a_k = a_n;
    }

    return a_n;
}
