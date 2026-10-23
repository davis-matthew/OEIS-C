/*
 * A042579  Denominators of continued fraction convergents to sqrt(818).
 * https://oeis.org/A042579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth continued‑fraction convergent of
 *            sqrt(818)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For sqrt(818) the floor part a0 = 28. */
    const idx_t D   = 818;
    const idx_t a0  = 28;          /* floor(sqrt(818)) */

    /* Base case n = 0: the first convergent has denominator 1. */
    if (n == 0)
        return (seq_t)1;

    /* Continued‑fraction recurrence variables. */
    idx_t m = (idx_t)0;   /* m0 */
    idx_t d = (idx_t)1;   /* d0 */
    idx_t a = a0;         /* current partial quotient */

    /* Convergent denominators: q_{-2} = 1, q_{-1} = 0. */
    seq_t q_prev2 = (seq_t)1;
    seq_t q_prev1 = (seq_t)0;
    seq_t q_curr;

    for (idx_t i = 0; i <= n; ++i)
    {
        /* Compute current denominator q_i. */
        q_curr = (seq_t)a * q_prev1 + q_prev2;
        if (i == n)
            return q_curr;

        /* Prepare next partial quotient a_{i+1}. */
        idx_t m_next = d * a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* Shift recurrence state. */
        q_prev2 = q_prev1;
        q_prev1 = q_curr;

        /* Advance the continued‑fraction state. */
        m = m_next;
        d = d_next;
        a = a_next;
    }
    /* Unreachable, but required for compilation. */
    return (seq_t)0;
}
