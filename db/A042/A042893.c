/*
 * A042893  Denominators of continued fraction convergents to sqrt(978).
 * https://oeis.org/A042893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) is the denominator of the n-th convergent of sqrt(978). It
 *            is computed using the standard continued‑fraction recurrence
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const unsigned long long N  = 978ULL;   /* D = 978          */
    const unsigned long long a0 = 31ULL;    /* floor(sqrt(978)) */

    unsigned long long m = 0;                /* m_0              */
    unsigned long long d = 1;                /* d_0              */
    unsigned long long a = a0;               /* a_0              */

    seq_t q_prev_prev = 0;  /* q_-1  */
    seq_t q_prev      = 1;  /* q_0   */
    seq_t q_next;

    for (idx_t i = 1; i <= n; ++i)
    {
        unsigned long long m_next = d * a - m;
        unsigned long long d_next = (N - m_next * m_next) / d;
        unsigned long long a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a = a_next;

        q_next = a * q_prev + q_prev_prev;
        q_prev_prev = q_prev;
        q_prev = q_next;
    }
    return q_prev;
}
