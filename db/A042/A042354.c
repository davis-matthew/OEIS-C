/*
 * A042354  Numerators of continued fraction convergents to sqrt(704).
 * https://oeis.org/A042354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 27, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of the continued‑fraction convergents to sqrt(704) */
/* Computed by the standard recurrence:
 *   p_{-2}=0, p_{-1}=1
 *   p_k = a_k * p_{k-1} + p_{k-2}
 *   a_0 = floor(sqrt(704)) = 26
 *   m_{k+1} = d_k * a_k - m_k
 *   d_{k+1} = (704 - m_{k+1}²) / d_k
 *   a_{k+1} = (a_0 + m_{k+1}) / d_{k+1}
 * The function recomputes the sequence from scratch for each call
 * (O(n) time, O(1) memory). */

seq_t a(idx_t n)
{
    /* constants for the quadratic irrational */
    const idx_t D  = 704;   /* D = 704 = sqrt(704)² */
    const idx_t a0 = 26;    /* a0 = floor(sqrt(704)) */

    /* continued‑fraction state */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a_coef = a0;      /* a_k for the current k */

    /* previous two convergent numerators (p_{k-2}, p_{k-1}) */
    seq_t p_prevprev = 0;
    seq_t p_prev     = 1;

    for (idx_t k = 0; k <= n; ++k)
    {
        /* numerator of the k-th convergent */
        seq_t p = (seq_t)a_coef * p_prev + p_prevprev;

        if (k == n)
            return p;          /* answer found */

        /* prepare next partial quotient and numerator */
        idx_t m1 = d * a_coef - m;
        idx_t d1 = (D - m1 * m1) / d;
        idx_t a_next = (a0 + m1) / d1;

        /* shift state for next iteration */
        m = m1;
        d = d1;
        a_coef = a_next;

        p_prevprev = p_prev;
        p_prev = p;
    }

    /* unreachable, but keeps the compiler happy */
    return (seq_t)0;
}
