/*
 * A042352  Numerators of continued fraction convergents to sqrt(703).
 * https://oeis.org/A042352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the nth convergent of the continued fraction
 *            of sqrt(703). a(0) = 26, a(1) = 27, a(2) = 53, ... as defined in
 *            OEIS A042352
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const uint64_t D   = 703ULL;   /* the number under the square root           */
    const uint64_t a0  = 26;       /* floor(sqrt(703))                            */

    if (n == 0) return (seq_t)a0;

    /* Continued‑fraction state ------------------------------------------------- */
    uint64_t m = 0, d = 1;           /* m_0, d_0                                  */

    /* Compute the first partial quotient a1 and advance the state */
    uint64_t m_next = d * a0 - m;                 /* = 26 */
    uint64_t d_next = (D - m_next * m_next) / d;   /* = 27 */
    uint64_t a_k    = (a0 + m_next) / d_next;      /* = 1  */

    /* Update for the next step */
    m = m_next;
    d = d_next;

    /* Numerator recurrence ----------------------------------------------------- */
    seq_t p_prevprev = (seq_t)1;      /* p_{-1}                                             */
    seq_t p_prev     = (seq_t)a0;     /* p_{0}  */
    seq_t p_cur      = (seq_t)a_k * p_prev + p_prevprev;   /* p_{1}  */

    if (n == 1) return p_cur;

    /* Shift values for the next iteration */
    p_prevprev = p_prev;
    p_prev    = p_cur;

    /* Iterate k = 2 .. n ------------------------------------------------------- */
    for (idx_t k = 2; k <= n; ++k) {
        /* Compute next partial quotient and update state */
        m_next = d * a_k - m;
        d_next = (D - m_next * m_next) / d;
        a_k    = (a0 + m_next) / d_next;

        /* Compute numerator p_k */
        p_cur = (seq_t)a_k * p_prev + p_prevprev;

        /* Shift for next step */
        p_prevprev = p_prev;
        p_prev     = p_cur;

        /* Prepare state for next iteration */
        m = m_next;
        d = d_next;
    }

    return p_cur;
}
