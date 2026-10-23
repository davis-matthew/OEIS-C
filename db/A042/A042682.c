/*
 * A042682  Numerators of continued fraction convergents to sqrt(871).
 * https://oeis.org/A042682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 20, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Numerators of continued‑fraction convergents to sqrt(871).
 *
 *  The continued fraction of √871 has the period
 *  [29; 1, 1, 19, 1, 1, 58] repeating.
 *  Recurrence:
 *
 *      p₋₁ = 1,   p₀ = 29
 *      m₀ = 0,    d₀ = 1
 *      a₀ = ⌊√871⌋ = 29
 *
 *  For k ≥ 1:
 *      m_k   = d_{k‑1}·a_{k‑1} – m_{k‑1}
 *      d_k   = (871 – m_k²) / d_{k‑1}
 *      a_k   = ⌊(29 + m_k) / d_k⌋
 *      p_k   = a_k·p_{k‑1} + p_{k‑2}
 *
 *  a(n) = p_n  (with a(0) = 29)
 */

seq_t a(idx_t n)
{
    const seq_t a0 = 29;          /* first partial quotient (floor(sqrt(871))) */
    const idx_t N = 871;          /* the radicand */

    if (n == 0) {
        return a0;
    }

    /* Initial values for the recurrence */
    seq_t p_prevprev = 1;        /* p_{-1} */
    seq_t p_prev     = a0;       /* p_0 */

    idx_t m = 0;
    idx_t d = 1;
    seq_t a_prev = a0;           /* a_{-1} is effectively a0 */

    for (idx_t i = 1; i <= n; ++i) {
        /*  Compute next partial quotient using the classic CF iteration */
        idx_t m_next = (idx_t)((seq_t)d * a_prev) - m;
        idx_t d_next = (N - m_next * m_next) / d;
        seq_t a_next = (seq_t)(a0 + (seq_t)m_next) / (seq_t)d_next;

        /*  Update continued‑fraction numerator p_k = a_k·p_{k‑1} + p_{k‑2} */
        seq_t p_next = a_next * p_prev + p_prevprev;

        /*  Shift for next iteration */
        p_prevprev = p_prev;
        p_prev     = p_next;
        m = m_next;
        d = d_next;
        a_prev = a_next;
    }

    return p_prev;   /* this is p_n = a(n) */
}
