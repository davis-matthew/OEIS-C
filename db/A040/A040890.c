/*
 * A040890  Continued fraction for sqrt(921).
 * https://oeis.org/A040890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) from continued fraction of sqrt(921). a(0) = 30. Subsequent
 *            terms repeat with period 34:
 *            [2,1,6,1,11,3,1,2,2,3,1,1,1,1,1,8,20,8,1,1,1,1,1,3,2,2,1,3,11,1,6,1,2,60]
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static period array and its length */
    static seq_t period[200];
    static size_t period_len = 0;

    /* first term */
    static seq_t first_term = (seq_t)30;

    if (n == 0)
        return first_term;

    /* compute the period only once */
    if (period_len == 0) {
        uint64_t a0 = 30ULL;     /* floor(sqrt(921)) */
        uint64_t D  = 921ULL;    /* the radicand */
        uint64_t m  = 0;
        uint64_t d  = 1;
        uint64_t a  = a0;

        /* generate the continued–fraction terms until the final 2*a0 */
        while (1) {
            uint64_t m_next = d * a - m;
            uint64_t d_next = (D - m_next * m_next) / d;
            uint64_t a_next = (a0 + m_next) / d_next;

            period[period_len++] = (seq_t)a_next;

            /* 2*a0 is the last term of the period */
            if (a_next == a0 * 2)
                break;

            m = m_next;
            d = d_next;
            a = a_next;
        }
    }

    /* index within the period */
    size_t pos = (size_t)((n - 1) % period_len);
    return period[pos];
}
