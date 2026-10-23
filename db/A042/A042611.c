/*
 * A042611  Denominators of continued fraction convergents to sqrt(834).
 * https://oeis.org/A042611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=1; for n≥1, a(n) is the denominator of the n‑th convergent
 *            of the continued fraction of √834. The recurrence follows from
 *            the partial quotients of √834: let a1=1, a2=7, a3=3, a4=1, a5=2,
 *            … (period 22, with a22=56). Then a(n) = a_n * a(n‑1) + a(n‑2)
 *            with a(−1)=0, a(0)=1
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 834 = 28² + 50, so a0 (floor sqrt) is 28 */
    const idx_t a0 = 28;
    const idx_t D  = 834;

    /* base convergents: q_{–1}=0, q_0=1 */
    seq_t q_prevprev = (seq_t)0;   /* q_{i-2} */
    seq_t q_prev     = (seq_t)1;   /* q_{i-1} */

    if (n == 0) return q_prev; /* q_0 = 1 */

    /* state used to generate continued‑fraction coefficients of √834 */
    idx_t m = (idx_t)0;
    idx_t d = (idx_t)1;
    idx_t a = a0;    /* a current (will be replaced by a_{i} each loop) */

    for (idx_t i = (idx_t)1; i <= n; ++i) {
        /* generate next coefficient a_{i} */
        idx_t m_next = d * a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* update state for next iteration */
        m = m_next;
        d = d_next;
        a = a_next;

        /* recurrence for denominator q_i */
        seq_t q_i = (seq_t)a * q_prev + q_prevprev;

        /* shift for next iteration */
        q_prevprev = q_prev;
        q_prev = q_i;
    }
    return q_prev;
}
