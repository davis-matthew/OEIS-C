/*
 * A042840  Numerators of continued fraction convergents to sqrt(951).
 * https://oeis.org/A042840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n‑th convergent to sqrt(951).
 *            Recurrence: a(0)=30, a(1)=31. For n≥2,
 *            a(n)=k[n-1]*a(n-1)+a(n-2), where k[i] are the partial quotients
 *            from the continued fraction of sqrt(951). The sequence of k[i]
 *            begins 30,1,5,? … (the usual Algorithm for √N continued
 *            fractions)
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t N   = 951;   /* the radicand */
    const seq_t a0  = 30;    /* floor(√N) */

    /* Handle n = 0 directly */
    if (n == 0)
        return a0;

    seq_t p_minus2 = 0;   /* p[-2]  */
    seq_t p_minus1 = 1;   /* p[-1]  */

    /* Variables for the continued fraction algorithm */
    seq_t m   = 0;
    seq_t d   = 1;
    seq_t a_cf = a0;          /* a[0] */

    seq_t p_curr;

    for (idx_t i = 0; i <= n; ++i) {
        /* Compute the numerator of the current convergent */
        p_curr = a_cf * p_minus1 + p_minus2;

        /* Shift for next iteration */
        p_minus2 = p_minus1;
        p_minus1 = p_curr;

        /* If we have reached the required index, break */
        if (i == n)
            break;

        /* Compute the next partial quotient a_cf */
        seq_t m_next = d * a_cf - m;
        seq_t d_next = (N - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        m   = m_next;
        d   = d_next;
        a_cf = a_next;
    }

    return p_curr;
}
