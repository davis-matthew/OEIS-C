/*
 * A010140  Continued fraction for sqrt(54).
 * https://oeis.org/A010140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 7 For n ≥ 1, the continued fraction for √54 is generated
 *            by a0 = 7 // floor(√54) m_const = 54 m = 0, d = 1, a = a0
 *            Repeatedly apply: m_next = d * a - m d_next = (m_const - m_next
 *            * m_next) / d a_next = (a0 + m_next) / d_next After the k‑th
 *            iteration a = a_next and it is the (k)th term a(k)...
 * Range:     u8 n <= 9980, u16 n <= 9980, u32 n <= 9980, u64 n <= 9980,
 *            u128 n <= 9980, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)7;
    }
    const seq_t a0 = (seq_t)7;
    const seq_t m_const = (seq_t)54;
    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;
    seq_t a_term = a0;
    idx_t i = 1;
    while (i <= n) {
        seq_t m_next = d * a_term - m;
        seq_t d_next = (m_const - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;
        if (i == n) {
            return a_next;
        }
        m = m_next;
        d = d_next;
        a_term = a_next;
        ++i;
    }
    /* unreachable, but to silence compiler warnings */
    return (seq_t)0;
}
