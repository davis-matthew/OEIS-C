/*
 * A040311  Continued fraction for sqrt(330).
 * https://oeis.org/A040311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = the n-th term of the continued fraction expansion of
 *            sqrt(330) (a(0) = floor(sqrt(330)) = 18; thereafter the period
 *            is [6,36] repeating)
 * Range:     u8 none, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"
#include <stddef.h>

seq_t a(idx_t n) {
    /* Periodic part of the continued fraction for sqrt(330). */
    static seq_t period[16];
    static size_t period_len = 0;

    if (period_len == 0) {
        const seq_t D = 330;          /* Radicand */
        seq_t a0 = floor_sqrt(D);     /* Integer part of sqrt(D) */
        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0;

        /* Compute the periodic part until the term 2*a0 occurs. */
        do {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;
            period[period_len++] = a;
        } while (a != 2 * a0);
    }

    /* a(0) is the integer part; otherwise return the periodic term. */
    if (n == 0) {
        return floor_sqrt(330);  /* a0 */
    }
    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}
