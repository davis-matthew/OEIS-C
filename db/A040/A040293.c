/*
 * A040293  Continued fraction for sqrt(311).
 * https://oeis.org/A040293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = ⌊√311⌋ = 17. For n > 0, a(n) is the (n‑1)th term of the
 *            repeating part of the simple continued fraction of √311. The
 *            period length is 16 (coefficients:
 *            1,1,1,2,1,6,3,17,3,6,1,2,1,1,1,34)
 * Range:     u8 none, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* static data initialized on first entry */
    static bool init = false;
    static seq_t period[64];
    static size_t period_len;
    static idx_t a0_idx;

    if (!init) {
        /* Compute a0 = floor(sqrt(311)) */
        a0_idx = (idx_t)floor_sqrt((seq_t)311);

        /* Build the periodic part of the continued fraction */
        idx_t D = 311;
        idx_t m = 0;
        idx_t d = 1;
        idx_t a_val = a0_idx;
        period_len = 0;

        while (1) {
            idx_t m_next = d * a_val - m;
            idx_t d_next = (D - m_next * m_next) / d;
            idx_t a_next = (a0_idx + m_next) / d_next;

            period[period_len++] = (seq_t)a_next;

            if (a_next == 2 * a0_idx)     /* end of period */
                break;

            m = m_next;
            d = d_next;
            a_val = a_next;
        }
        init = true;
    }

    if (n == 0)
        return (seq_t)a0_idx;

    /* Periodic part: shift by 1 because a(1) is the first period element */
    idx_t pos = (n - 1) % period_len;
    return period[pos];
}
