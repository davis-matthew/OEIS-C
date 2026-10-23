/*
 * A042063  Denominators of continued fraction convergents to sqrt(555).
 * https://oeis.org/A042063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 44, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of convergents to sqrt(555) continued fraction
   Computed by recurrence q_{-1}=0, q_0=1, q_{k+1}=a_k*q_k+q_{k-1}
   with partial quotients obtained from the standard continued‑fraction
   expansion of sqrt(555). */

seq_t a(idx_t n)
{
    /* D = 555; floor(sqrt(D)) = 23 */
    const idx_t D   = 555;
    const idx_t a0  = 23;

    if (n == 0)
        return (seq_t)1;   /* q0 = 1 */

    /* state of the continued fraction algorithm */
    idx_t m = 0;          /* m_0   */
    idx_t d = 1;          /* d_0   */
    idx_t a_cur = a0;     /* a_k   */

    /* recurrence for denominators */
    seq_t q_prev = (seq_t)0;   /* q_{-1} */
    seq_t q_curr = (seq_t)1;   /* q_0    */

    for (idx_t k = 0; ; ++k)
    {
        /* Update q for k >= 1 */
        if (k > 0)
        {
            seq_t q_next = a_cur * q_curr + q_prev;
            q_prev = q_curr;
            q_curr = q_next;
        }

        if (k == n)
            return q_curr;

        /* Compute next partial quotient a_{k+1} */
        idx_t m_next = d * a_cur - m;
        idx_t d_next = (D - m_next * m_next) / d;
        m = m_next;
        d = d_next;
        a_cur = (a0 + m) / d;
    }
}
