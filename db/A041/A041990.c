/*
 * A041990  Numerators of continued fraction convergents to sqrt(518).
 * https://oeis.org/A041990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 14, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numerators of continued fraction convergents to sqrt(518) */
seq_t a(idx_t n)
{
    /* Continued fraction of sqrt(518): a0 = 22 */
    const seq_t D = 518;       /* Discriminant */
    const seq_t a0 = 22;       /* Initial integer part */

    seq_t m = 0;  /* m0 */
    seq_t d = 1;  /* d0 */
    seq_t a_k = a0;               /* a_k for current k */
    seq_t p_prevprev = 0;         /* p_{-2} */
    seq_t p_prev = 1;             /* p_{-1} */
    seq_t p_cur = 0;              /* p_k */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* compute current convergent numerator p_k */
        p_cur = a_k * p_prev + p_prevprev;

        /* update for next iteration */
        if (k == n)
            break;

        p_prevprev = p_prev;
        p_prev = p_cur;

        /* advance to a_{k+1} */
        m = d * a_k - m;
        d = (seq_t)((D - m * m) / d);
        a_k = (a0 + m) / d;
    }

    return p_cur;
}
