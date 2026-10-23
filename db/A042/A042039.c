/*
 * A042039  Denominators of continued fraction convergents to sqrt(543).
 * https://oeis.org/A042039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042039: Denominators of continued‑fraction convergents to √543.
   The convergents satisfy
       q₋₂ = 1,  q₋₁ = 0,
       qₙ   = aₙ·qₙ₋₁ + qₙ₋₂,
   where aₙ are the coefficients of the continued fraction of √543.
   This function implements that recurrence directly. */
seq_t a(idx_t n)
{
    /* Parameters of the continued fraction of √543 */
    const idx_t D = 543;          /* the radicand */
    const idx_t a0 = 23;          /* floor(√543) */

    /* Variables for the continued fraction method */
    idx_t m = 0, d = 1, a_k = a0;

    /* Convergent denominators: q₋₂, q₋₁, qₙ */
    seq_t q_pp = 1, q_p = 0, q = 0;

    for (idx_t i = 0; i <= n; ++i)
    {
        /* Compute current convergent denominator q */
        q = (seq_t)a_k * q_p + q_pp;

        if (i == n)
            return q;          /* a(n) found */

        /* Prepare for next iteration */
        idx_t m_next = d * a_k - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* rotate past two denominators */
        q_pp = q_p;
        q_p  = q;

        /* advance continued‑fraction variables */
        m = m_next;
        d = d_next;
        a_k = a_next;
    }

    /* unreachable, but keep compiler happy */
    return q;
}
