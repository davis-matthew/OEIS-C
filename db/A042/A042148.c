/*
 * A042148  Numerators of continued fraction convergents to sqrt(599).
 * https://oeis.org/A042148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = numerator of the nth convergent of the continued fraction
 *            expansion of sqrt(599). The sequence is calculated by the
 *            standard recurrence for convergents of a quadratic irrational
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A042148. */
seq_t a(idx_t n)
{
    /* 599 is the non‑square D for sqrt(D).  a0 = floor(sqrt(599)) = 24. */
    idx_t a0 = 0;
    while ((a0 + 1) * (a0 + 1) <= 599) a0++;

    /* Variables for the continued‑fraction algorithm.  All are safe
       in 64‑bit arithmetic because D = 599 is small. */
    idx_t m   = 0;      /* current m */
    idx_t d   = 1;      /* current d */
    idx_t a_next = 0;   /* next partial quotient (for i > 0) */

    /* Convergent numerators p_{-2} = 0, p_{-1} = 1.  These two
       values are used to compute successive numerators. */
    seq_t p_prevprev = 0;
    seq_t p_prev     = 1;

    for (idx_t i = 0; i <= n; ++i) {
        idx_t a_i = (i == 0) ? a0 : a_next;          /* current partial quotient   */

        /* Compute current numerator: p_i = a_i * p_{i-1} + p_{i-2} */
        seq_t p = (seq_t)a_i * p_prev + p_prevprev;

        /* Shift, so p_prevprev = p_{i-1}, p_prev = p_i */
        p_prevprev = p_prev;
        p_prev     = p;

        /* Prepare partial quotient for the next iteration. */
        if (i < n) {
            idx_t m_next  = d * a_i - m;
            idx_t d_next  = (599 - m_next * m_next) / d;
            idx_t a_next_ = (a0 + m_next) / d_next;

            m   = m_next;
            d   = d_next;
            a_next = a_next_;
        }
    }

    /* p_prev now contains the numerator of the nth convergent. */
    return p_prev;
}
