/*
 * A042719  Denominators of continued fraction convergents to sqrt(889).
 * https://oeis.org/A042719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 21, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued‑fraction recurrence for the denominators of convergents to sqrt(889).
   a(0) = 1, a(1) = 1, a(2) = 5, … (see OEIS A042719). */

seq_t a(idx_t n)
{
    /* Constant √889                     */
    const seq_t S   = (seq_t)889;   /* S = D */
    const seq_t a0  = (seq_t)29;    /* a0 = floor(√889) */

    /* Handle the very first term directly */
    if (n == 0) {
        return (seq_t)1;           /* q0 = 1 */
    }

    /* Variables for the continued‑fraction algorithm */
    seq_t m     = (seq_t)0;   /* m0   */
    seq_t d     = (seq_t)1;   /* d0   */
    seq_t a_k   = a0;         /* current a_k; starts with a0 for computing a1 */

    /* Convergent denominators: q_{-1} = 0, q_0 = 1 */
    seq_t q_prevprev = (seq_t)0;  /* q_{k-2}   */
    seq_t q_prev     = (seq_t)1;  /* q_{k-1}   */
    seq_t q_k;                    /* q_k for current k */

    /* Iterate from k = 1 to k = n to compute q_n */
    for (idx_t k = 1; k <= n; ++k) {
        /* Compute next m, d, and a using the current a_k (which is a_k-1) */
        seq_t m_next = d * a_k - m;
        seq_t d_next = (S - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        /* Compute the denominator of the k‑th convergent */
        q_k = a_next * q_prev + q_prevprev;

        /* Advance all variables for the next step */
        m     = m_next;
        d     = d_next;
        a_k   = a_next;
        q_prevprev = q_prev;
        q_prev     = q_k;
    }

    return q_k;
}
