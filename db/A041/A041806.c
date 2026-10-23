/*
 * A041806  Numerators of continued fraction convergents to sqrt(424).
 * https://oeis.org/A041806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of n-th convergent to sqrt(424). Recurrence:
 *            p_{-2}=0, p_{-1}=1, p_k = c_k*p_{k-1}+p_{k-2} (k>=0) where c_k
 *            are the partial quotients of sqrt(424) obtained by the standard
 *            continued–fraction algorithm: a0 = ⌊√424⌋ = 20, m0 = 0, d0 = 1,
 *            a0 = 20, m_{k+1} = d_k*a_k - m_k, d_{k+1} =...
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 20, u64 n <= 42, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t N = 424;      /* target square */
    const seq_t a0 = 20;      /* floor(sqrt(424)) */

    /* Initial continued–fraction state for k = 0 */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a_k = a0;

    /* Convergent recurrence state: p_{-2} = 0, p_{-1} = 1 */
    seq_t p_nm2 = 0;                 /* p_{k-2}  */
    seq_t p_nm1 = 1;                 /* p_{k-1}  */

    idx_t k = 0;                     /* current index */

    for (;;) {
        /* Compute the numerator for index k */
        seq_t p_k = a_k * p_nm1 + p_nm2;
        if (k == n) return p_k;

        /* Advance to the next partial quotient a_{k+1} */
        seq_t m_next = d * a_k - m;
        seq_t tmp     = N - m_next * m_next;
        seq_t d_next  = tmp / d;
        seq_t a_next  = (a0 + m_next) / d_next;

        /* Shift state for next iteration */
        m = m_next;
        d = d_next;
        a_k = a_next;

        p_nm2 = p_nm1;
        p_nm1 = p_k;

        ++k;
    }
}
