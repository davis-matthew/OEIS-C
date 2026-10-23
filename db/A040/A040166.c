/*
 * A040166  Continued fraction for sqrt(180).
 * https://oeis.org/A040166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction partial quotients of sqrt(180). */
/* a(0) = 13, a(1..∞) follow the pattern 2,2,2,26 repeating. */

seq_t a(idx_t n)
{
    /* The integer part of sqrt(180). */
    const seq_t A0 = 13;

    /* a(0) is the first term. */
    if (n == 0)
        return A0;

    /* State variables for the continued fraction iteration. */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a_val = A0;

    idx_t i;
    for (i = 1; i <= n; ++i) {
        seq_t m_next = d * a_val - m;
        seq_t d_next = ((seq_t)180 - m_next * m_next) / d;
        seq_t a_next = (A0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a_val = a_next;
    }

    return a_val;
}
