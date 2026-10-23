/*
 * A042686  Numerators of continued fraction convergents to sqrt(873).
 * https://oeis.org/A042686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 19, u64 n <= 40, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(873)
   a(0) = 29, a(1) = 30, a(2) = 59, ...
   Computed by iterating the continued–fraction expansion of sqrt(873). */
seq_t a(idx_t n)
{
    const seq_t D   = 873;        /* the integer whose sqrt we approximate   */
    const seq_t a0  = 29;         /* floor(sqrt(D))                           */
    seq_t m = 0;                  /* current m in continued–fraction algorithm */
    seq_t d = 1;                  /* current d                               */
    seq_t a_k = a0;               /* current continued fraction coefficient   */

    /* convergent recurrence:   p[-2] = 0, p[-1] = 1  */
    seq_t p_prev2 = 0;
    seq_t p_prev1 = 1;
    seq_t p_curr;

    for (idx_t i = 0; ; ++i)
    {
        /* compute current numerator */
        p_curr = a_k * p_prev1 + p_prev2;
        if (i == n)
            return p_curr;

        /* shift for next step */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;

        /* compute next a_k via the standard continued‑fraction update */
        seq_t m_next = d * a_k - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a_k = a_next;
    }
}
