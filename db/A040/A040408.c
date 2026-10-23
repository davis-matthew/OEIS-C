/*
 * A040408  Continued fraction for sqrt(429).
 * https://oeis.org/A040408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Continued fraction of sqrt(429)
 *  a(0) = floor(sqrt(429)) = 20
 *  For n ≥ 1, the recurrence (moving from a_k to a_{k+1}) is:
 *      m_{k+1} = d_k * a_k – m_k
 *      d_{k+1} = (429 – m_{k+1}²) / d_k
 *      a_{k+1} = (20 + m_{k+1}) / d_{k+1}
 *  Those three integers (m, d, a) are updated exactly as above.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 20;  /* a(0) */

    const seq_t a0 = 20;  /* floor(sqrt(429)) */
    seq_t m = 0;          /* m_0 */
    seq_t d = 1;          /* d_0 */
    seq_t a_val = a0;     /* a_0 */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* m_{k+1} = d_k * a_k – m_k */
        unsigned long long prod = (unsigned long long)d * (unsigned long long)a_val;
        m = (seq_t)(prod - (unsigned long long)m);

        /* d_{k+1} = (429 – m_{k+1}²) / d_k */
        unsigned long long mm   = (unsigned long long)m * (unsigned long long)m;
        unsigned long long diff = (unsigned long long)429 - mm;
        d = (seq_t)(diff / (unsigned long long)d);

        /* a_{k+1} = (20 + m_{k+1}) / d_{k+1} */
        a_val = (a0 + m) / d;
    }

    return a_val;
}
