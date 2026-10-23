/*
 * A042395  Denominators of continued fraction convergents to sqrt(724).
 * https://oeis.org/A042395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n‑th convergent to sqrt(724)
 *            Recurrence derived from the continued‑fraction algorithm: let a₀
 *            = floor(sqrt(724)) = 26; with m₀ = 0, d₀ = 1, q₋₁ = 0, q₀ = 1.
 *            For k ≥ 1: m_k = dₖ₋₁ * aₖ₋₁ - mₖ₋₁ d_k = (724 - m_k²) / dₖ₋₁
 *            a_k = (a₀ + m_k) / d_k (integer division) q_k = a_k...
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 23, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t D   = 724;
    const seq_t a0  = 26;     /* floor(sqrt(724)) */
    seq_t m   = 0, d = 1, a_term = a0;
    seq_t q_prev = 0, q_curr = 1;   /* q_{-1} = 0, q_0 = 1 */

    if (n == 0)
        return q_curr;

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t m_next = d * a_term - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a_term = a_next;

        seq_t q_next = a_term * q_curr + q_prev;
        q_prev = q_curr;
        q_curr = q_next;
    }
    return q_curr;
}
