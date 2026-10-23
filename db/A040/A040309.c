/*
 * A040309  Continued fraction for sqrt(328).
 * https://oeis.org/A040309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = n-th term of the continued fraction expansion of
 *            sqrt(328)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm uses the standard recurrence for simple continued fractions: */
/*   m_{k+1} = d_k*a_k - m_k                     */
/*   d_{k+1} = (D - m_{k+1}^2) / d_k             */
/*   a_{k+1} = floor((a0 + m_{k+1}) / d_{k+1})   */
/* with D = 328 and a0 = floor(sqrt(328)) = 18. */

#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t D  = 328ULL;
    const uint64_t a0 = 18;          /* floor(sqrt(328)) */
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = a0;

    if (n == 0) return (seq_t)a0;

    for (idx_t i = 1; i <= n; ++i)
    {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
    }

    return (seq_t)a;
}
