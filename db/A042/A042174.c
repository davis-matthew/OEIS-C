/*
 * A042174  Numerators of continued fraction convergents to sqrt(612).
 * https://oeis.org/A042174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 19, u64 n <= 39, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numerators of convergents to √612.  */
/* Recurrence:  p_{-1}=1, p_0=24,   p_n = a_n·p_{n-1}+p_{n-2}  */
/* where a_n are the continued‑fraction partial quotients of √612. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)24;

    seq_t p_prev_prev = (seq_t)1;    /* p_{-1} */
    seq_t p_prev     = (seq_t)24;    /* p_0   */
    seq_t m          = (seq_t)0;
    seq_t d          = (seq_t)1;
    seq_t a_cur      = (seq_t)24;

    for (idx_t i = 0; i < n; ++i) {
        seq_t m_next = d * a_cur - m;
        seq_t d_next = ((seq_t)612 - m_next * m_next) / d;
        seq_t a_next = ((seq_t)24 + m_next) / d_next;

        seq_t p_next = a_next * p_prev + p_prev_prev;

        p_prev_prev = p_prev;
        p_prev     = p_next;

        m = m_next;
        d = d_next;
        a_cur = a_next;
    }
    return p_prev;
}
