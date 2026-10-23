/*
 * A041744  Numerators of continued fraction convergents to sqrt(392).
 * https://oeis.org/A041744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of convergents to sqrt(392) (OEIS A041744). */
/* Recurrence: a(0)=19; for n≥1 let
 *   m_n = d_{n-1} * a_{n-1} - m_{n-1}
 *   d_n = (392 - m_n^2) / d_{n-1}
 *   a_n = floor((19 + m_n)/d_n)
 *   p_{-2}=0, p_{-1}=1
 *   p_n = a_n * p_{n-1} + p_{n-2}
 * Returns p_n. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)19;
    const seq_t a0 = 19;
    seq_t m = 0;
    seq_t d = 1;
    seq_t a_term = a0;
    seq_t p_minus2 = 0;
    seq_t p_minus1 = 1;
    seq_t p_curr = a0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t m_next = d * a_term - m;
        seq_t d_next = ((seq_t)392 - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;
        seq_t p_next = a_next * p_curr + p_minus1;
        m = m_next;
        d = d_next;
        a_term = a_next;
        p_minus2 = p_minus1;
        p_minus1 = p_curr;
        p_curr = p_next;
    }
    return p_curr;
}
