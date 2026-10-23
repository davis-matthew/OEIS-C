/*
 * A042492  Numerators of continued fraction convergents to sqrt(774).
 * https://oeis.org/A042492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 41, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the numerators of the continued‑fraction convergents to √774
   (OEIS A042492).  The function uses the standard recurrence for the
   convergents of a quadratic irrational:
        p_{-1}=1,  p_0 = ⌊√774⌋ = 27
        p_n   = a_n * p_{n-1} + p_{n-2}
   where a_n are the partial quotients of the continued fraction of √774.
   The partial quotients are generated on the fly using the well‑known
   recurrence for the continued fraction of √D. */

seq_t a(idx_t n)
{
    /* initial partial quotient a0 = floor(sqrt(774)) */
    const seq_t A0 = (seq_t)27;

    /* numerators of the convergents: p_{-1} and p_0 */
    seq_t p_prevprev = (seq_t)1;
    seq_t p_prev     = A0;

    /* variables for the continued fraction expansion of √774 */
    idx_t m = 0;            /* m_0 */
    idx_t d = 1;            /* d_0 */
    idx_t a_prev = 27;      /* a_0 */

    if (n == 0)
        return p_prev;

    /* iterate to compute a_n and p_n for 1 ≤ n ≤ n */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* compute next m_k and d_k */
        idx_t m_next = d * a_prev - m;
        /* avoid overflow: 774 < 2^10, so m_next*m_next fits in 64 bits */
        idx_t d_next = (774 - m_next * m_next) / d;

        /* compute next partial quotient a_k */
        idx_t a_k = (27 + m_next) / d_next;

        /* compute next numerator p_k */
        seq_t p_k = (seq_t)a_k * p_prev + p_prevprev;

        /* shift variables for next iteration */
        p_prevprev = p_prev;
        p_prev     = p_k;
        m          = m_next;
        d          = d_next;
        a_prev     = a_k;
    }

    return p_prev;
}
