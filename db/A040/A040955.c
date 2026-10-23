/*
 * A040955  Continued fraction for sqrt(987).
 * https://oeis.org/A040955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms of sqrt(987) */
/* a(0) = floor(sqrt(987)); thereafter the period repeats with a(1)..a(4) = 2,2,2,62 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t a0;
    static seq_t period[256];
    static idx_t period_len;

    if (!init) {
        /* initial term */
        a0 = floor_sqrt((seq_t)987);

        /* compute the periodic part of a continued fraction */
        seq_t N = (seq_t)987;
        seq_t m = 0;
        seq_t d = 1;
        seq_t a_term = a0;
        idx_t cnt = 0;

        while (1) {
            /* next triple (m,d,a) */
            m = d * a_term - m;
            d = (N - m * m) / d;
            a_term = (a0 + m) / d;

            period[cnt++] = a_term;
            if (a_term == 2 * a0)           /* period ends */
                break;
        }
        period_len = cnt;
        init = true;
    }

    if (n == 0)
        return a0;

    idx_t idx = (n - 1) % period_len;
    return period[idx];
}
