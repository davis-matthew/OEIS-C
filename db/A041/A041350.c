/*
 * A041350  Numerators of continued fraction convergents to sqrt(189).
 * https://oeis.org/A041350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 35, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction convergents numerators for sqrt(189) */

seq_t a(idx_t n)
{
    /* sqrt(189) = 3*sqrt(21), the continued fraction coefficients are generated
       using the classical recurrence:
         m_k = d_{k-1} * a_{k-1} - m_{k-1}
         d_k = (S - m_k^2) / d_{k-1}
         a_k = (a0 + m_k) / d_k
       The numerators p_k satisfy
         p_k = a_k * p_{k-1} + p_{k-2}
       with p_{-2}=0, p_{-1}=1.  */
    const idx_t S = (idx_t)189;          /* the radicand */
    const idx_t a0 = 13;                 /* floor(sqrt(189)) */

    /* State for the continued‑fraction recurrence */
    idx_t m = 0, d = 1, a = a0;

    /* State for the convergent numerator recurrence */
    seq_t p_minus2 = 0;  /* p_{k-2} */
    seq_t p_minus1 = 1;  /* p_{k-1} */
    seq_t p = 0;         /* current numerator */

    for (idx_t i = 0; i <= n; ++i)
    {
        /* compute current numerator */
        p = (seq_t)a * p_minus1 + p_minus2;
        if (i == n)
            return p;

        /* update continued‑fraction state for next coefficient */
        idx_t m_next = d * a - m;
        idx_t d_next = (S - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* shift convergent numerator state */
        p_minus2 = p_minus1;
        p_minus1 = p;

        /* prepare for next iteration */
        m = m_next;
        d = d_next;
        a = a_next;
    }
    /* Should never be reached */
    return 0;
}
