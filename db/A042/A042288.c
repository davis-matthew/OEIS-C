/*
 * A042288  Numerators of continued fraction convergents to sqrt(670).
 * https://oeis.org/A042288
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

/* OEIS sequence A042288: Numerators of continued‑fraction convergents to sqrt(670)
   a(n) = numerator of the n-th convergent of √670.  */
seq_t a(idx_t n)
{
    const idx_t D  = 670;      /* D = 670 */
    const idx_t a0 = 25;       /* floor(√670) = 25 */

    /* Current partial quotient (a_i), and the standard
       variables for the continued‑fraction iteration. */
    idx_t m = 0;   /* m_0 */
    idx_t d = 1;   /* d_0 */
    idx_t ai = a0; /* a_0 = 25 */

    /* Convergent numerators:  p_{-2}=0, p_{-1}=1  */
    seq_t p_pp = 0;  /* p_{-2} */
    seq_t p_p  = 1;  /* p_{-1} */
    seq_t curr = 0;  /* p_n */

    for (idx_t i = 0; i <= n; ++i)
    {
        /* p_n = a_i * p_{n-1} + p_{n-2} */
        curr = (seq_t)ai * p_p + p_pp;
        if (i == n)
            return curr;

        /* Compute the next partial quotient a_{i+1} */
        idx_t m_next = d * ai - m;
        idx_t d_next = (D - m_next * m_next) / d;
        ai = (a0 + m_next) / d_next;

        /* Shift the numerator history for next iteration */
        p_pp = p_p;
        p_p  = curr;
        m    = m_next;
        d    = d_next;
    }
    /* The loop always returns before this point. */
    return 0;
}
