/*
 * A040878  Continued fraction for sqrt(909).
 * https://oeis.org/A040878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) is the nth partial quotient in the continued fraction
 *            expansion of sqrt(909)
 * Range:     u8 none, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   floor_sqrt, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/floor_sqrt.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static const seq_t D = 909;
    static seq_t a0 = 0;
    static seq_t *period = NULL;
    static size_t period_len = 0;
    static size_t period_cap = 0;

    if (period_len == 0) {
        a0 = floor_sqrt(D);

        period_cap = 16;
        period = seq_alloc(period_cap);

        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0;

        while (1) {
            seq_t m_next = d * a - m;
            seq_t d_next = (D - m_next * m_next) / d;
            seq_t a_next = (a0 + m_next) / d_next;

            if (period_len == period_cap) {
                period_cap <<= 1;
                period = seq_realloc(period, period_len, period_cap);
            }
            period[period_len++] = a_next;

            if (a_next == 2 * a0) {
                break;
            }
            m = m_next;
            d = d_next;
            a = a_next;
        }
    }

    if (n == 0) {
        return a0;
    }

    idx_t pos = (n - 1) % period_len;
    return period[(size_t)pos];
}
