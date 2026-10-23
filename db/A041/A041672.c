/*
 * A041672  Numerators of continued fraction convergents to sqrt(355).
 * https://oeis.org/A041672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerators of continued fraction convergents of
 *            sqrt(355). This is computed using the standard
 *            continued‑fraction recurrences: m_{k+1} = d_k * a_k - m_k
 *            d_{k+1} = (S - m_{k+1}^2) / d_k a_{k+1} = floor((a_0 + m_{k+1})
 *            / d_{k+1}) with S = 355 and a_0 = floor(sqrt(355)) = 18.
 *            Convergents (p_k...
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t S = (seq_t)355;          /* square root argument     */
    const seq_t a0 = (seq_t)18;          /* floor(sqrt(355))         */

    /* State of the continued fraction generator.  We start at the first
       term (k = 0).  m_0 = 0, d_0 = 1, a_0 = floor(sqrt(S)). */
    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;
    seq_t a_k = a0;                     /* a current partial quotient   */

    /* Numerators of convergents:  p_{-2} = 0,  p_{-1} = 1.   */
    seq_t p_prev2 = (seq_t)0;           /* p_{k-2}   */
    seq_t p_prev1 = (seq_t)1;           /* p_{k-1}   */
    seq_t p_curr  = (seq_t)0;           /* p_k       */

    /* Compute p_0 = a_0 * p_{-1} + p_{-2}.  This is the first term. */
    p_curr = a0 * p_prev1 + p_prev2;
    if (n == (idx_t)0) return p_curr;

    /* Generate subsequent convergents up to a_n. */
    for (idx_t k = (idx_t)1; k <= n; ++k)
    {
        /* Next values of the CF components.  All arithmetic is
           done in seq_t, so overflow is avoided for the arbitrary
           precision build. */
        const seq_t m_next = d * a_k - m;
        const seq_t d_next = (S - m_next * m_next) / d;
        const seq_t a_next = (a0 + m_next) / d_next;  /* floor division */

        /* Update the numerators using the recurrence p_k = a_k * p_{k-1} + p_{k-2}. */
        const seq_t p_next = a_next * p_curr + p_prev1;

        /* Shift state for the next iteration. */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
        p_curr  = p_next;

        /* Prepare for next step. */
        m = m_next;
        d = d_next;
        a_k = a_next;
    }

    return p_curr;   /* p_n */
}
