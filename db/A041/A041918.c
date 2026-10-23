/*
 * A041918  Numerators of continued fraction convergents to sqrt(481).
 * https://oeis.org/A041918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 18, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(481).
   The sequence satisfies
        p_{-2} = 0,  p_{-1} = 1,
        p_k   = a_k * p_{k-1} + p_{k-2},
   where a_0 = ⌊√481⌋ = 21 and the subsequent a_k are obtained by the
   classical quadratic form recurrence for the continued fraction of √481. */
seq_t a(idx_t n) {
    const seq_t A0 = 21;           /* floor(sqrt(481)) */
    const seq_t D  = 481;          /* the integer under the square root    */

    if (n == 0) {                  /* first term a(0) = 21 */
        return A0;
    }

    /* continued‑fraction state: m_k, d_k, a_k for k = current index   */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a_k = A0;                /* a_0 already used for the first numerator */

    /* convergent numerators */
    seq_t p_prevprev = 0;          /* p_{-2} */
    seq_t p_prev     = 1;          /* p_{-1} */
    seq_t p_cur      = A0;         /* p_0   */

    for (idx_t i = 1; i <= n; ++i) {
        /* compute next a_i via the recurrence for quadratic irrationals */
        seq_t m_next = d * a_k - m;
        seq_t d_next = (D - m_next * m_next) / d;
        seq_t a_next = (A0 + m_next) / d_next;

        /* update the numerator with the convergent recurrence */
        seq_t p_next = a_next * p_cur + p_prev;

        /* shift variables for the next iteration */
        p_prevprev = p_prev;
        p_prev     = p_cur;
        p_cur      = p_next;

        m   = m_next;
        d   = d_next;
        a_k = a_next;
    }

    return p_cur;
}
