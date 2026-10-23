/*
 * A040466  Continued fraction for sqrt(489).
 * https://oeis.org/A040466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients for sqrt(489).
   a(0)=22, subsequent terms repeat with period length 22:
   [8,1,4,1,1,1,3,2,1,2,14,2,1,2,3,1,1,1,4,1,8,44] */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    static seq_t period[128];
    static size_t period_len = 0;
    static seq_t a0_local = 0;
    const seq_t N = (seq_t)489;

    if (period_len == 0) {
        /* Compute the integer part of sqrt(489) */
        a0_local = floor_sqrt(N);

        /* Generate the period of the continued fraction. */
        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0_local;
        size_t idx = 0;
        while (1) {
            seq_t m_next = d * a - m;
            seq_t d_next = (N - m_next * m_next) / d;
            seq_t a_next = (a0_local + m_next) / d_next;
            period[idx++] = a_next;

            if (a_next == a0_local * 2) {
                /* 2*a0 marks the end of the period. */
                break;
            }
            m = m_next;
            d = d_next;
            a = a_next;
        }
        period_len = idx;
    }

    if (n == 0) {
        return a0_local;
    }

    size_t pos = (size_t)(n - 1) % period_len;
    return period[pos];
}
