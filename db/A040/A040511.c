/*
 * A040511  Continued fraction for sqrt(535).
 * https://oeis.org/A040511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) gives the nth term of the simple continued fraction
 *            expansion of √535. The expansion is periodic: a(0) = ⌊√535⌋ =
 *            23, followed by the period 7, 1, 2, 4, 1, 3, 1, 4, 2, 1, 7, 46
 *            (which ends with 2·23). The algorithm below computes the period
 *            once using the standard continued‑fraction algorith...
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    static seq_t period[64];
    static idx_t period_len = 0;

    /* compute the period only once */
    if (period_len == 0) {
        const idx_t D = 535;
        idx_t a0 = (idx_t)floor_sqrt((seq_t)D);    /* a0 = ⌊√D⌋ */

        idx_t m = 0;
        idx_t d = 1;
        idx_t a_cur = a0;

        while (1) {
            /* m_{k+1} = d_k * a_k - m_k */
            m = d * a_cur - m;
            /* d_{k+1} = (D - m_{k+1}^2) / d_k */
            d = (D - m * m) / d;
            /* a_{k+1} = ⌊(a0 + m_{k+1}) / d_{k+1}⌋ */
            a_cur = (a0 + m) / d;

            period[period_len++] = (seq_t)a_cur;   /* store the term */

            if (a_cur == 2 * a0)           /* period finished (last term 2*a0) */
                break;
        }
    }

    if (n == 0) {
        /* first term is the integer part of the sqrt */
        return (seq_t)floor_sqrt((seq_t)535);
    }

    /* otherwise return the (n-1)-th period entry */
    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}
