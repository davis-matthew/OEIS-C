/*
 * A040315  Continued fraction for sqrt(334).
 * https://oeis.org/A040315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = Continued fraction partial quotients of sqrt(334) (a(0) =
 *            floor(sqrt(334)) = 18, subsequent a(n) for n >= 1 are given by
 *            the recurrence used in the continued‑fraction algorithm)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Cache the period of the continued fraction for sqrt(334). */
    static seq_t period[28];
    static size_t period_len = 0;
    static bool initialized = false;

    if (!initialized) {
        /* D = 334, a0 = floor(sqrt(334)) = 18. */
        const uint64_t D   = 334;
        const uint64_t a0  = 18;
        uint64_t m = 0;          /* m_0 */
        uint64_t d = 1;          /* d_0 */
        uint64_t cur_a = a0;     /* a_0 */
        size_t i = 0;

        while (1) {
            uint64_t m_next = d * cur_a - m;
            uint64_t d_next = (D - m_next * m_next) / d;
            uint64_t a_next = (a0 + m_next) / d_next;

            period[i++] = (seq_t)a_next;

            if (a_next == 2 * a0) {
                period_len = i;
                break;          /* a_next == 2*a0 signals period end */
            }

            m = m_next;
            d = d_next;
            cur_a = a_next;
        }
        initialized = true;
    }

    if (n == 0) {
        return (seq_t)18;          /* a(0) = floor(sqrt(334)) */
    } else {
        /* Periodic part starts from a(1). */
        size_t idx = (size_t)((idx_t)n - 1) % period_len;
        return period[idx];
    }
}
