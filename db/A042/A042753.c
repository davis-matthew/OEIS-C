/*
 * A042753  Denominators of continued fraction convergents to sqrt(907).
 * https://oeis.org/A042753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) is the denominator of the nth convergent to sqrt(907).
 *            Denominators satisfy the recurrence: a(0) = 1, a(1) = 8, a(n) =
 *            c(n-1) * a(n-1) + a(n-2) (n ≥ 2) where c(k) is the (k+1)-th
 *            partial quotient of the continued fraction expansion of
 *            sqrt(907)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth denominator of the convergents to sqrt(907). */
seq_t a(idx_t n)
{
    /* Quick return for the first term. */
    if (n == 0)
        return (seq_t)1;

    /* Parameters for the continued fraction of sqrt(907). */
    const idx_t D   = 907;   /* The radicand */
    const idx_t a0  = 30;    /* floor(sqrt(907)) */

    /* State for generating continued fraction terms: m, d, a_k. */
    idx_t m   = 0;
    idx_t d   = 1;
    idx_t a_k = a0;   /* a_0, unused for denominators after q_0 */

    /* Recurrence for denominators: q_{-1}=0, q_0=1. */
    seq_t q_prev2 = 0;  /* q_{-1} */
    seq_t q_prev1 = 1;  /* q_0  */
    seq_t q_curr  = 0;  /* placeholder */

    /* Generate terms up to the requested index. */
    for (idx_t i = 1; i <= n; ++i) {
        /* Next continued fraction coefficient a_i. */
        idx_t m_next = d * a_k - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* Denominator recurrence using the newly obtained coefficient. */
        q_curr = (seq_t)a_next * q_prev1 + q_prev2;

        /* Advance indices and state. */
        q_prev2 = q_prev1;
        q_prev1 = q_curr;
        m = m_next;
        d = d_next;
        a_k = a_next;
    }

    return q_curr;
}
