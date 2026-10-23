/*
 * A041878  Numerators of continued fraction convergents to sqrt(461).
 * https://oeis.org/A041878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 16, u64 n <= 35, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(461). 
   a(n) is the nth numerator in the convergents sequence. */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)21;

    const idx_t D = 461;
    const idx_t sqrtD = 21;          /* ⌊√D⌋ */

    idx_t m = 0;
    idx_t d = 1;
    idx_t cur_a = sqrtD;             /* a₀ = 21 */

    seq_t p_prevprev = 1;            /* p_{-1} */
    seq_t p_prev = 21;               /* p₀ */

    for (idx_t i = 1; i <= n; ++i) {
        /* compute next partial quotient aᵢ */
        idx_t m_next = d * cur_a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (sqrtD + m_next) / d_next;

        /* recurrence for convergent numerators: pᵢ = aᵢ · p_{i-1} + p_{i-2} */
        seq_t p_next = (seq_t)a_next * p_prev + p_prevprev;

        /* shift for next iteration */
        p_prevprev = p_prev;
        p_prev = p_next;
        m = m_next;
        d = d_next;
        cur_a = a_next;
    }

    return p_prev;
}
