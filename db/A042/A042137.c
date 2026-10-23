/*
 * A042137  Denominators of continued fraction convergents to sqrt(593).
 * https://oeis.org/A042137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042137: Denominators of continued fraction convergents to sqrt(593).
   Compute the n‑th denominator by iterating the standard continued–fraction
   recurrence.  No table of values is used. */
seq_t a(idx_t n) {
    const idx_t D = 593;          /* radicand */
    const idx_t a0 = 24;          /* floor(sqrt(593)) */

    /* Convergents: p_{-2}=0, p_{-1}=1 ; q_{-2}=1, q_{-1}=0
       Here we need only the q’s. */
    seq_t prev2 = (seq_t)1;  /* q_{-2} */
    seq_t prev1 = (seq_t)0;  /* q_{-1} */
    seq_t cur    = (seq_t)0; /* current q */

    /* Continued–fraction state variables */
    idx_t m  = (idx_t)0;
    idx_t d  = (idx_t)1;
    idx_t a  = a0;          /* current partial quotient a_k */

    for (idx_t k = 0; k <= n; ++k) {
        /* q_k = a_k * q_{k-1} + q_{k-2} */
        cur = (seq_t)a * prev1 + prev2;
        if (k == n) break;  /* finished */

        /* update to next partial quotient a_{k+1} */
        idx_t m_next = d * a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* shift members for next iteration */
        m = m_next;
        d = d_next;
        a = a_next;

        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
