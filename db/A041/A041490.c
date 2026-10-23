/*
 * A041490  Numerators of continued fraction convergents to sqrt(262).
 * https://oeis.org/A041490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041490: Numerators of continued‑fraction convergents of sqrt(262).
   Recurrence: p_{-1}=1, p_0=16, p_k = a_k * p_{k-1} + p_{k-2},
   where a_k are the partial quotients of sqrt(262). */

seq_t a(idx_t n)
{
    /* First partial quotient a0 = floor(sqrt(262)) */
    const idx_t a0 = 16;
    if (n == 0) {
        return (seq_t)a0;
    }

    /* Convergent numerators: p_{-1}=1, p_0=16 */
    seq_t p_im2 = (seq_t)1;      /* p_{k-2} */
    seq_t p_im1 = (seq_t)a0;     /* p_{k-1} */

    /* Continued‑fraction state for sqrt(262) */
    idx_t m      = 0;            /* m_0 */
    idx_t d      = 1;            /* d_0 */
    idx_t a_prev = a0;           /* a_0 */

    /* Iterate k = 1 .. n */
    for (idx_t k = 1; k <= n; ++k) {
        /* compute next partial quotient a_k */
        idx_t m_next = d * a_prev - m;
        idx_t d_next = (262 - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* update convergent numerator */
        seq_t a_next_seq = (seq_t)a_next;
        seq_t p_k = a_next_seq * p_im1 + p_im2;

        /* shift for next iteration */
        p_im2 = p_im1;
        p_im1 = p_k;

        /* advance continued‑fraction state */
        m      = m_next;
        d      = d_next;
        a_prev = a_next;
    }

    return p_im1;
}
