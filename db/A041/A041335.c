/*
 * A041335  Denominators of continued fraction convergents to sqrt(181).
 * https://oeis.org/A041335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   Denominators of continued‑fraction convergents to sqrt(181) *
 *            Computation uses the standard continued‑fraction algorithm. *
 *            a(0) = 1, a(1) = 2, a(2) = 9, a(3) = 11,
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return the first denominator directly */
    if (n == 0) return (seq_t)1;          /* q0 = 1 */

    /* Integer part of sqrt(181) */
    const idx_t A0 = 13;    /* floor(sqrt(181)) */
    const idx_t S  = 181;   /* radicand */

    /* Variables for generating the continued‑fraction partial quotients */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a_k = A0;          /* a_0 */

    /* Convergent recurrence variables:
     * p_{-1} = 1, q_{-1} = 0, p_0 = a0, q_0 = 1   */
    seq_t p_prev = (seq_t)1; /* p_{-1} */
    seq_t q_prev = (seq_t)0; /* q_{-1} */
    seq_t p_cur  = (seq_t)A0;/* p_0   */
    seq_t q_cur  = (seq_t)1; /* q_0   */

    for (idx_t k = 1; k <= n; ++k) {
        /* Next partial quotient a_{k} */
        m     = d * a_k - m;
        d     = (S - m * m) / d;
        a_k   = (A0 + m) / d;

        /* Convergent recurrence for numerator and denominator */
        seq_t p_next = (seq_t)a_k * p_cur + p_prev;
        seq_t q_next = (seq_t)a_k * q_cur + q_prev;

        /* Shift for next iteration */
        p_prev = p_cur;  q_prev = q_cur;
        p_cur  = p_next; q_cur  = q_next;
    }
    return q_cur;          /* q_n  */
}
