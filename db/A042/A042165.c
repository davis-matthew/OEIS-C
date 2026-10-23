/*
 * A042165  Denominators of continued fraction convergents to sqrt(607).
 * https://oeis.org/A042165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued fraction convergents to sqrt(607) */
seq_t a(idx_t n)
{
    const idx_t D     = 607;          /* the quadratic irrational square root */ 
    const idx_t a0    = 24;           /* floor(sqrt(D)) */ 
    /* State variables for the continued fraction expansion */
    idx_t m   = 0;                    /* m_k     */
    idx_t d   = 1;                    /* d_k     */
    idx_t a_k = a0;                   /* a_k     */

    /* Recurrence for convergents: q_-2 = 1, q_-1 = 0 */
    seq_t q_prev2 = (seq_t)1;          /* q_k-2   */
    seq_t q_prev1 = (seq_t)0;          /* q_k-1   */
    seq_t q_curr;                     /* q_k     */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* Compute the k-th denominator */
        q_curr = (seq_t)a_k * q_prev1 + q_prev2;
        if (k == n) return q_curr;

        /* Shift the previous two denominators */
        q_prev2 = q_prev1;
        q_prev1 = q_curr;

        /* Generate the next continued‑fraction element */
        idx_t m_next = d * a_k - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        m   = m_next;
        d   = d_next;
        a_k = a_next;
    }
    /* Never reached for valid n */
    return (seq_t)0;
}
