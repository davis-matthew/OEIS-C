/*
 * A010205  Continued fraction for sqrt(154).
 * https://oeis.org/A010205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   Continued fraction terms for sqrt(154)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* D = 154 is not a perfect square; its continued fraction is
       [12; overline{2,2,3,1,2,1,3,2,2,24}] */
    const unsigned long long D   = 154ULL;
    const unsigned long long a0  = 12ULL;   /* floor(sqrt(154)) */

    if (n == 0) return (seq_t)a0;  /* a(0) = 12 */

    unsigned long long m   = 0ULL;   /* m_0   */
    unsigned long long d   = 1ULL;   /* d_0   */
    unsigned long long a_p = a0;     /* a_{k-1} */

    idx_t i;
    for (i = 1; i <= n; ++i) {
        unsigned long long m_next = d * a_p - m;
        unsigned long long d_next = (D - m_next * m_next) / d;
        unsigned long long a_next = (a0 + m_next) / d_next;

        if (i == n)
            return (seq_t)a_next;  /* cast fits into seq_t */

        m   = m_next;
        d   = d_next;
        a_p = a_next;
    }

    /* Should never reach here */
    return (seq_t)0;
}
