/*
 * A041900  Numerators of continued fraction convergents to sqrt(472).
 * https://oeis.org/A041900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = numerator of the nth convergent of the continued fraction
 *            of sqrt(472). Recurrence: p_{-2} = 0, p_{-1} = 1 a0 = ⌊√472⌋ =
 *            21 For n ≥ 0: p_n = a_n * p_{n-1} + p_{n-2} where a_n are the
 *            partial quotients of sqrt(472): a0 = 21, a1 = 1, a2 = 2, a3 = 1,
 *            a4 = 1, a5 = 1, a6 = 4, a7 = 5, a8 = 4, a9...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* constants for the quadratic surd sqrt(472) */
    const unsigned long long D  = 472ULL;   /* D > 0, not a perfect square */
    const unsigned long long a0 = 21ULL;    /* floor(sqrt(D)) */

    /* continued fraction state: m, d, a */
    unsigned long long m = 0ULL;
    unsigned long long d = 1ULL;
    unsigned long long a = a0; /* a0 is used for n = 0 */

    /* convergent numerators: p_{-2}, p_{-1}, p_n */
    seq_t p_m2 = 0;     /* p_{-2} */
    seq_t p_m1 = 1;     /* p_{-1} */
    seq_t p_n  = 0;     /* current numerator */

    for (idx_t i = 0; i <= n; ++i)
    {
        /* compute p_n = a * p_{n-1} + p_{n-2} */
        p_n = (seq_t)a * p_m1 + p_m2;

        if (i == n)
            return p_n;

        /* advance to the next partial quotient a_{i+1} */
        unsigned long long m_next = d * a - m;
        unsigned long long d_next = (D - m_next * m_next) / d;
        unsigned long long a_next = (a0 + m_next) / d_next;

        /* rotate numerator states */
        p_m2 = p_m1;
        p_m1 = p_n;

        /* update continued fraction state */
        m = m_next;
        d = d_next;
        a = a_next;
    }

    /* unreachable, but required to silence compiler warnings */
    return 0;
}
