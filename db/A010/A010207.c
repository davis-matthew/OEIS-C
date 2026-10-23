/*
 * A010207  Continued fraction for sqrt(158).
 * https://oeis.org/A010207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Continued fraction for √158.
 * a(0) = 12
 * For n ≥ 1: a(n) repeats the cycle [1, 1, 3, 12, 3, 1, 1, 24]
 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t period[64];
    static unsigned period_len = 0;
    static bool initialized = false;
    const seq_t D   = 158;
    const seq_t a0  = 12;   /* floor(sqrt(158)) */

    /* Compute period once */
    if (!initialized) {
        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0;
        while (true) {
            /* next terms of the continued fraction of sqrt(D) */
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;
            period[period_len++] = a;
            if (a == 2 * a0)          /* full period finished */
                break;
            if (period_len >= sizeof(period)/sizeof(period[0]))
                break;               /* safety guard, should not happen */
        }
        initialized = true;
    }

    if (n == 0)
        return a0;                    /* first term */

    /* n ≥ 1: use periodic part */
    idx_t k = (n - 1) % (idx_t)period_len;
    return period[k];
}
