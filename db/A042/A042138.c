/*
 * A042138  Numerators of continued fraction convergents to sqrt(594).
 * https://oeis.org/A042138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = numerator of the n‑th convergent of √594. Let a0 = ⌊√594⌋
 *            = 24. Define p(-2)=0, p(-1)=1 and for k≥0 p(k) = a_k · p(k-1) +
 *            p(k-2), where a_k are the continued‑fraction coefficients of
 *            √594 (obtained by the standard m,d recurrence)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t a0 = 24;          /* integer part of √594 */

    /* previous two convergent numerators: p(-2)=0, p(-1)=1 */
    seq_t p_prev2 = 0;           /* p(-2) */
    seq_t p_prev1 = 1;           /* p(-1) */
    seq_t p_cur;

    /* state for the continued‑fraction expansion of √594 */
    idx_t m = 0;                 /* m_0 */
    idx_t d = 1;                 /* d_0 */
    idx_t a_k = a0;              /* a_0 */

    for (idx_t k = 0; k <= n; ++k) {
        if (k == 0) {
            p_cur = (seq_t)a_k;          /* p(0) = a0 */
        } else {
            p_cur = (seq_t)a_k * p_prev1 + p_prev2;
        }

        if (k == n) {
            return p_cur;
        }

        /* shift numerators for next step */
        p_prev2 = p_prev1;
        p_prev1 = p_cur;

        /* generate next coefficient a_{k+1} */
        idx_t m_next = d * a_k - m;
        idx_t d_next = (594 - m_next * m_next) / d;
        a_k = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
    }

    /* unreachable, but satisfies compiler requirements */
    return 0;
}
