/*
 * A042204  Numerators of continued fraction convergents to sqrt(628).
 * https://oeis.org/A042204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n‑th convergent of sqrt(628)
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 17, u64 n <= 33, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the standard continued fraction expansion of sqrt(628). */
seq_t a(idx_t n) {
    /* sqrt(628) ≈ 25.059…; its continued fraction has partial quotients
     * that repeat with a period; here we generate them on the fly.    */
    const idx_t D  = 628;   /* the square‑free part of the radicand */
    const idx_t a0 = 25;    /* floor(sqrt(D))                          */

    /* Base case:  p_0 = a0 */
    if (n == 0) {
        return (seq_t)a0;
    }

    /* Continued‑fraction data (m, d, a_k).  Initially m_0 = 0, d_0 = 1,
     * a_0 = floor(sqrt(D)). */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a_k = a0;  /* current partial quotient a_k = a_{k} */

    /* Continuants (numerators of convergents)
     * p_{-1} = 1,  p_0 = a0                         */
    seq_t p_im2 = 1;             /* p_{k-2}   */
    seq_t p_im1 = (seq_t)a0;     /* p_{k-1}   */
    seq_t p_k;                   /* current p_k */

    for (idx_t i = 1; i <= n; ++i) {
        /* NEXT partial quotient via the standard recurrence   */
        idx_t m_next = d * a_k - m;                     /* m_{k+1}          */
        idx_t d_next = (D - m_next * m_next) / d;       /* d_{k+1}          */
        idx_t a_next = (a0 + m_next) / d_next;          /* a_{k+1}          */

        /* CONTINUANT recurrence: p_k = a_next * p_{k-1} + p_{k-2} */
        p_k = (seq_t)a_next * p_im1 + p_im2;

        /* Shift for next iteration */
        p_im2 = p_im1;
        p_im1 = p_k;

        m = m_next;
        d = d_next;
        a_k = a_next;
    }

    return p_im1;   /* This is p_n, the numerator of the n‑th convergent */
}
