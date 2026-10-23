/*
 * A040654  Continued fraction for sqrt(681).
 * https://oeis.org/A040654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = the n-th partial quotient in the continued fraction expansion of sqrt(681). */

seq_t a(idx_t n) {
    /* constants for sqrt(681) */
    const seq_t D  = 681;   /* D = 681 */
    const seq_t a0 = 26;    /* floor(sqrt(681)) = 26 */

    if (n == 0)
        return a0;

    /* recurrence variables */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t m_next = d * a - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;
        m = m_next;
        d = d_next;
        a = a_next;
    }

    return a;
}
