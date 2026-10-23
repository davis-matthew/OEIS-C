/*
 * A042764  Numerators of continued fraction convergents to sqrt(913).
 * https://oeis.org/A042764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent to sqrt(913) in its
 *            continued fraction expansion
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 19, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* constants for sqrt(913) */
    const seq_t N      = 913;   /* value of the square root */
    const seq_t sqrtN  = 30;    /* floor(sqrt(913)) -- the first partial quotient */

    /* trivial case */
    if (n == 0)
        return sqrtN;

    /* recurrence variables for convergents:  p_{-1}=1, p_0=sqrtN */
    seq_t p_prev = (seq_t)1;    /* p_{k-2} */
    seq_t p_curr = sqrtN;       /* p_{k-1} */

    /* variables for generating the continued‑fraction terms */
    seq_t m = (seq_t)0;         /* m_0 */
    seq_t d = (seq_t)1;         /* d_0 */
    seq_t a_k = sqrtN;          /* a_0 */

    /* iterate n times to compute p_n (the desired numerator) */
    for (idx_t i = 1; i <= n; ++i) {
        /* Next (m,d,a) values for the continued fraction of sqrt(913) */
        seq_t m_next = d * a_k - m;
        seq_t d_next = (N - m_next * m_next) / d;
        seq_t a_next = (sqrtN + m_next) / d_next;

        /* Update the convergent numerator using the recurrence
           p_k = a_k * p_{k-1} + p_{k-2} */
        seq_t p_next = a_next * p_curr + p_prev;

        /* Shift for the next iteration */
        p_prev = p_curr;
        p_curr = p_next;

        m = m_next;
        d = d_next;
        a_k = a_next;
    }
    return p_curr;
}
