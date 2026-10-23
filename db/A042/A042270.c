/*
 * A042270  Numerators of continued fraction convergents to sqrt(661).
 * https://oeis.org/A042270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent to sqrt(661) (continued
 *            fraction)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const int A0 = 25;          /* floor(sqrt(661)) */
    static const uint64_t D = 661;     /* The radicand */

    uint64_t m = 0;
    uint64_t d = 1;
    int a_cf = A0;                     /* current continued fraction term */

    seq_t p_minus2 = 0;                /* p_{-2} */
    seq_t p_minus1 = 1;                /* p_{-1} */

    for (idx_t k = 0; ; ++k) {
        seq_t curr = (seq_t)a_cf * p_minus1 + p_minus2;
        if (k == n)
            return curr;

        /* advance to next continued fraction term */
        uint64_t m_next = d * (uint64_t)a_cf - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        a_cf = (A0 + (int)m_next) / (int)d_next;

        /* shift previous numerators */
        p_minus2 = p_minus1;
        p_minus1 = curr;

        /* update m, d for next term */
        m = m_next;
        d = d_next;
    }
}
