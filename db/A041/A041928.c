/*
 * A041928  Numerators of continued fraction convergents to sqrt(487).
 * https://oeis.org/A041928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(0)=22, a(1)=309, a(n)=p_n where p_{-2}=0, p_{-1}=1 and p_n =
 *            a_n * p_{n-1} + p_{n-2} (a_n is the n‑th partial quotient of the
 *            continued fraction of sqrt(487).)
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 18, u64 n <= 36, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Constants for sqrt(487) */
    const seq_t a0 = 22;          /* floor(sqrt(487)) */
    const seq_t D   = 487;        /* the non‑square D */

    /* Reconstruct the n-th convergent numerator directly. */
    seq_t p_prev2 = 0;            /* p_{-2} */
    seq_t p_prev1 = 1;            /* p_{-1} */
    seq_t m = 0;                  /* m_0 */
    seq_t d = 1;                  /* d_0 */

    for (idx_t k = 0; ; ++k)
    {
        seq_t q;                  /* a_k */
        if (k == 0)
            q = a0;
        else
            q = (a0 + m) / d;     /* floor((a0 + m)/d) */

        seq_t p_curr = q * p_prev1 + p_prev2;

        if (k == n)              /* requested term */
            return p_curr;

        /* Prepare for next k (k < n).  Update m,d using current q. */
        seq_t m_new = d * q - m;
        seq_t d_new = (D - m_new * m_new) / d;  /* always integer */
        m = m_new;
        d = d_new;

        /* Shift the previous convergents */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }
}
