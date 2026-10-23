/*
 * A040940  Continued fraction for sqrt(972).
 * https://oeis.org/A040940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = floor(sqrt(972)) = 31. For n > 0 generate the continued
 *            fraction expansion of sqrt(972) by the standard recurrence:
 *            m_{k+1} = d_k * a_k - m_k d_{k+1} = (972 - m_{k+1}^2) / d_k
 *            a_{k+1} = (31 + m_{k+1}) / d_{k+1} where a_0 = 31, m_0 = 0, d_0
 *            = 1. The function returns a(n)
 * Range:     u8 n <= 1, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t D      = 972;
    const seq_t a0     = 31;          /* floor(sqrt(D)) */
    if (n == 0)
        return a0;

    seq_t m = 0;
    seq_t d = 1;
    seq_t a_val = a0;

    for (idx_t i = 1; i <= n; ++i) {
        m = d * a_val - m;
        d = (D - m * m) / d;
        a_val = (a0 + m) / d;
    }

    return a_val;
}
