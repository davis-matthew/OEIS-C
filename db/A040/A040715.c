/*
 * A040715  Continued fraction for sqrt(743).
 * https://oeis.org/A040715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Continued‑fraction terms of sqrt(743).
   a(0) = 27, followed by the repeating block [3,1,7,27,7,1,3,54]. */

seq_t a(idx_t n)
{
    static seq_t period[200];
    static size_t period_len = 0;
    static bool initialized = false;
    static const seq_t a0 = 27;

    if (!initialized) {
        seq_t m = 0;
        seq_t d = 1;
        seq_t a_val = a0;
        do {
            seq_t m_next = d * a_val - m;
            seq_t d_next = (743 - m_next * m_next) / d;
            seq_t a_next = (a0 + m_next) / d_next;

            period[period_len++] = a_next;
            m = m_next;
            d = d_next;
            a_val = a_next;
        } while (a_val != 2 * a0);
        initialized = true;
    }

    if (n == 0)
        return a0;

    size_t idx = (size_t)(n - 1) % period_len;
    return period[idx];
}
