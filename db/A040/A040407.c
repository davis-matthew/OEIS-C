/*
 * A040407  Continued fraction for sqrt(428).
 * https://oeis.org/A040407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = n-th term of the simple continued fraction expansion of
 *            sqrt(428). * The algorithm follows the standard recurrence: *
 *            m_0 = 0, d_0 = 1, a_0 = floor(sqrt(428)) = 20 * m_{k+1} =
 *            d_k*a_k - m_k * d_{k+1} = (428 - m_{k+1}^2)/d_k * a_{k+1} =
 *            floor( (a_0 + m_{k+1}) / d_{k+1} ) * The function comp...
 * Range:     u8 n <= 1, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t N   = (seq_t)428;   /* Radicand */
    const seq_t a0  = (seq_t)20;    /* floor(sqrt(428)) */
    seq_t m = (seq_t)0;
    seq_t d = (seq_t)1;
    seq_t a = a0;

    if (n == 0) return a;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* Next state in the continued‑fraction algorithm */
        m = d * a - m;
        d = (N - m * m) / d;
        a = (a0 + m) / d;
    }
    return a;
}
