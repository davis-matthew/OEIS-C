/*
 * A040924  Continued fraction for sqrt(955).
 * https://oeis.org/A040924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   Continued fraction coefficients of sqrt(955) * (a(0) =
 *            floor(sqrt(955)) = 30; the remaining terms repeat with period
 *            16.)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for size_t, malloc, free */

seq_t a(idx_t n)
{
    /* Constants */
    const uint64_t D   = 955ULL;
    const uint64_t A0  = 30ULL;          /* floor(sqrt(955)) */

    /* Periodic part: stored once and reused for all calls */
    static seq_t  period[128];           /* plenty of space for the period */
    static size_t period_len = 0;        /* 0 indicates uninitialized */

    /* Initialize period on first invocation */
    if (period_len == 0) {
        uint64_t m = 0, d = 1, a_val = A0;
        size_t  i = 0;

        do {
            m = d * a_val - m;
            d = (D - m * m) / d;
            a_val = (A0 + m) / d;
            /* store a_val as seq_t; works for built‑in and arbitrary precision types */
            period[i++] = (seq_t)a_val;
        } while (a_val != 2 * A0);    /* last term of period is 2*A0 */
        period_len = i;
    }

    /* Return the requested term */
    if (n == 0) {
        return (seq_t)A0;
    } else {
        /* n-1 is guaranteed safe because n>0 */
        size_t pos = (size_t)((n - 1) % (idx_t)period_len);
        return period[pos];
    }
}
