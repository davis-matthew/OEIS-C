/*
 * A042449  Denominators of continued fraction convergents to sqrt(752).
 * https://oeis.org/A042449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 21, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042449: Denominators of continued fraction convergents to sqrt(752)
   Recurrence:
     a0 = floor(sqrt(752)) = 27
     m0 = 0, d0 = 1
     mk+1 = dk * ak - mk
     dk+1 = (752 - mk+1^2) / dk
     ak+1 = (a0 + mk+1) / dk+1
     q-2 = 1, q-1 = 0
     qk = ak * qk-1 + qk-2   (a(n) = qn)
*/

seq_t a(idx_t n)
{
    const unsigned int D = 752;                 /* constant sqrt argument   */

    /* compute integer part of sqrt(D) without floating point */
    unsigned int a0 = 0;
    while ((a0 + 1) * (a0 + 1) <= D) ++a0;      /* a0 = 27                */

    unsigned int m   = 0;        /* m0   */
    unsigned int d   = 1;        /* d0   */
    unsigned int a_k = a0;       /* a0   */

    seq_t p_nm2 = 0;   /* p_{-2} */
    seq_t p_nm1 = 1;   /* p_{-1} */
    seq_t q_nm2 = 1;   /* q_{-2} */
    seq_t q_nm1 = 0;   /* q_{-1} */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* current convergent */
        seq_t p_n = (seq_t)a_k * p_nm1 + p_nm2;
        seq_t q_n = (seq_t)a_k * q_nm1 + q_nm2;

        if (k == n)
            return q_n;

        /* shift for next step */
        p_nm2 = p_nm1;  p_nm1 = p_n;
        q_nm2 = q_nm1;  q_nm1 = q_n;

        /* next partial quotient */
        unsigned int m_next = d * a_k - m;
        unsigned int d_next = (D - m_next * m_next) / d;
        unsigned int a_next = (a0 + m_next) / d_next;

        m   = m_next;
        d   = d_next;
        a_k = a_next;
    }

    /* unreachable; return 0 as placeholder */
    return 0;
}
