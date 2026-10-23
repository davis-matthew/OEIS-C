/*
 * A040377  Continued fraction for sqrt(397).
 * https://oeis.org/A040377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction terms of sqrt(397)
 * Range:     u8 n <= 0, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 19, with period length 21 thereafter */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *period = NULL;
    static size_t period_len = 0;
    static const seq_t D = 397;
    static const seq_t a0 = 19;

    if (n == 0)
        return a0;

    if (period == NULL) {
        seq_t m = 0, d = 1, a_val = a0;
        size_t cap = 32;
        period = (seq_t *)malloc(cap * sizeof(seq_t));
        period_len = 0;
        while (1) {
            /* compute next m, d, a according to the continued‑fraction algorithm */
            m = d * a_val - m;
            d = (D - m * m) / d;
            a_val = (a0 + m) / d;
            if (period_len == cap) {
                cap <<= 1;
                period = (seq_t *)realloc(period, cap * sizeof(seq_t));
            }
            period[period_len++] = a_val;
            if (a_val == 2 * a0)   /* end of the periodic part */
                break;
        }
    }

    return period[(n - 1) % period_len];
}
