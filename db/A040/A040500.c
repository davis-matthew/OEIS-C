/*
 * A040500  Continued fraction for sqrt(523).
 * https://oeis.org/A040500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) = the n-th coefficient of the continued fraction of
 *            sqrt(523). Computed using the standard recurrence:
 *            m_{k+1}=d_k*a_k-m_k d_{k+1}=(N-m_{k+1}^2)/d_k
 *            a_{k+1}=⌊(a_0+m_{k+1})/d_{k+1}⌋ The sequence is periodic after
 *            a_0, with period ending when a_k = 2*a_0
 * Range:     u8 none, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    static bool init = false;
    static seq_t a0 = 0;
    static seq_t period[1024];
    static size_t period_len = 0;

    if (!init) {
        const seq_t N = (seq_t)523;
        a0 = floor_sqrt(N);

        seq_t m = 0, d = 1, a_k = a0;
        size_t i = 0;

        do {
            seq_t m_next = d * a_k - m;
            seq_t d_next = (N - m_next * m_next) / d;
            seq_t a_next = (a0 + m_next) / d_next;

            period[i++] = a_next;

            if (a_next == 2 * a0) break;

            m = m_next;
            d = d_next;
            a_k = a_next;
        } while (true);

        period_len = i;
        init = true;
    }

    if (n == 0)
        return a0;

    size_t idx = ((size_t)n - 1) % period_len;
    return period[idx];
}
