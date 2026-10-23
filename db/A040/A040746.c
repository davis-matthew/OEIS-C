/*
 * A040746  Continued fraction for sqrt(774).
 * https://oeis.org/A040746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) is the nth term of the continued‑fraction expansion of
 *            sqrt(774)
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static storage for period */
    static bool init = false;
    static seq_t period[128];
    static size_t period_len = 0;
    static seq_t a0 = 0;

    if (!init) {
        /* a0 = floor(sqrt(774)) */
        const seq_t D = (seq_t)774;
        seq_t t = 0;
        while ((t + (seq_t)1) * (t + (seq_t)1) <= D) {
            t = t + (seq_t)1;
        }
        a0 = t;

        /* compute the periodic part of the continued fraction */
        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0;
        do {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;
            period[period_len++] = a;
        } while (a != (a0 + a0));
        init = true;
    }

    if (n == 0) {
        return a0;
    }

    /* index into the period (period starts with a1) */
    size_t idx = (size_t)((n - 1) % (idx_t)period_len);
    return period[idx];
}
