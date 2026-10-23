/*
 * A040712  Continued fraction for sqrt(740).
 * https://oeis.org/A040712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = the nth term of the simple continued fraction expansion
 *            of sqrt(740). The expansion starts with a(0)=27 and repeats with
 *            period 6: 4,1,12,1,4,54. This implementation computes the terms
 *            analytically using the standard recurrence for quadratic surds,
 *            avoiding any hard‐coded lookup tables
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* number used for the continued fraction of sqrt(740) */
    const idx_t N   = 740;      /* N > seq_t may be 8‑bit, use idx_t */
    const idx_t a0  = 27;       /* floor(sqrt(740)) */

    if (n == 0) return (seq_t)a0;

    idx_t m   = 0;          /* initial m in the continued fraction algorithm */
    idx_t d   = 1;          /* initial d (always divides N - m^2) */
    idx_t a_i = a0;         /* current a_i value */

    for (idx_t i = 1; i <= n; ++i) {
        /* recurrence for quadratic surds */
        idx_t m_new = d * a_i - m;
        idx_t d_new = (N - m_new * m_new) / d;
        idx_t a_new = (a0 + m_new) / d_new;

        m   = m_new;
        d   = d_new;
        a_i = a_new;
    }

    return (seq_t)a_i;
}
