/*
 * A041777  Denominators of continued fraction convergents to sqrt(409).
 * https://oeis.org/A041777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Denominators of convergents to sqrt(409).
 * The n‑th term is the denominator of the n‑th convergent of the
 * simple continued fraction of √409.  The partial quotients are
 * produced by the standard recurrence for quadratic irrationals.
 */
seq_t a(idx_t n)
{
    /* Constants for the continued fraction of √409 */
    const seq_t D   = 409;   /* the radicand */
    const seq_t a0  = 20;    /* floor(√409) */

    if (n == 0) {
        return (seq_t)1;     /* q_0 = 1 */
    }

    /* recurrence variables for the denominators q_n */
    seq_t q_prevprev = 0;    /* q_{-1} */
    seq_t q_prev     = 1;    /* q_0 */

    /* variables for generating the partial quotients a_n */
    seq_t m   = 0;
    seq_t d   = 1;
    seq_t a_prev = a0;

    for (idx_t i = 1; i <= n; ++i) {
        /* next partial quotient a_i */
        seq_t m_new = d * a_prev - m;
        seq_t d_new = (D - m_new * m_new) / d;
        seq_t a_new = (a0 + m_new) / d_new;

        /* next denominator q_i = a_i * q_{i-1} + q_{i-2} */
        seq_t q = a_new * q_prev + q_prevprev;

        /* shift for next iteration */
        q_prevprev = q_prev;
        q_prev     = q;
        m = m_new;
        d = d_new;
        a_prev = a_new;
    }

    return q_prev;  /* q_n */
}
