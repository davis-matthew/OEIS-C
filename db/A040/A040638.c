/*
 * A040638  Continued fraction for sqrt(664).
 * https://oeis.org/A040638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction terms of sqrt(664)
 * Range:     u8 none, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t a0 = 0;
    static seq_t period[128];
    static size_t period_len = 0;

    if (period_len == 0) {
        const idx_t D = 664;          /* square‑free part of 664 */
        /* compute a0 = floor(sqrt(D)) */
        seq_t sqrt_start = 0;
        for (seq_t i = 0; (i + 1) * (i + 1) <= (seq_t)D; ++i) {
            sqrt_start = i + 1;
        }
        a0 = sqrt_start;

        /* compute periodic part using the classical algorithm */
        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0;

        while (true) {
            /* next iteration */
            m = d * a - m;
            d = (((seq_t)D - m * m) / d);
            a = (a0 + m) / d;   /* floor division */

            period[period_len++] = a;
            if (a == 2 * a0) break;   /* period ends */
        }
    }

    if (n == 0)
        return a0;

    /* periodic part: a(1) ... a(L) repeat */
    idx_t idx = (n - 1) % period_len;
    return period[(size_t)idx];
}
