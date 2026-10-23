/*
 * A040578  Continued fraction for sqrt(603).
 * https://oeis.org/A040578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) is the n‑th term of the continued fraction expansion of
 *            sqrt(603)
 * Range:     u8 none, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    static seq_t period[200];
    static idx_t period_len = 0;
    static seq_t a0;

    /* Compute the period of the continued fraction the first time */
    if (period_len == 0) {
        a0 = floor_sqrt((seq_t)603);
        seq_t m = 0;
        seq_t d = 1;
        seq_t a_cur = a0;
        idx_t i = 0;
        seq_t D = (seq_t)603;
        seq_t twice_a0 = a0 * 2;

        while (1) {
            m = d * a_cur - m;
            d = (D - m * m) / d;
            a_cur = (a0 + m) / d;
            period[i++] = a_cur;
            if (a_cur == twice_a0)
                break;
        }
        period_len = i;
    }

    if (n == 0)
        return a0;
    idx_t idx = (n - 1) % period_len;
    return period[idx];
}
