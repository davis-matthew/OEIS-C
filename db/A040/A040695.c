/*
 * A040695  Continued fraction for sqrt(722).
 * https://oeis.org/A040695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) is the nth coefficient of the continued fraction expansion
 *            of sqrt(722). a(0) = 26, and for n > 0 the period is
 *            [1,6,1,2,3,2,26,2,3,2,1,6,1,52]
 * Range:     u8 none, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    static seq_t period[32];
    static idx_t period_len = 0;
    static seq_t a0 = 0;

    /* Initialise period on first call */
    if (period_len == 0) {
        const seq_t D = (seq_t)722;
        a0 = floor_sqrt(D);

        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0; /* This is a0 already */

        /* Generate coefficients until 2*a0 appears */
        while (1) {
            seq_t m_next = a * d - m;
            seq_t d_next = (D - m_next * m_next) / d;
            seq_t a_next = (a0 + m_next) / d_next;

            period[period_len++] = a_next;
            if (a_next == a0 * (seq_t)2)
                break;

            m = m_next;
            d = d_next;
            a = a_next;
        }
    }

    if (n == 0)
        return a0;
    idx_t pos = (n - 1) % period_len;
    return period[pos];
}
