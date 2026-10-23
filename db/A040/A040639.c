/*
 * A040639  Continued fraction for sqrt(665).
 * https://oeis.org/A040639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms of sqrt(665).  a(0) = floor(sqrt(665)) = 25.
   The remaining terms repeat a period of length 10: (1,3,1,2,2,2,1,3,1,50). */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Cached state for the period */
    static bool initialized = false;
    static seq_t a0 = 0;                /* floor(sqrt(665)) */
    static seq_t period[2048];          /* holds the repeating block */
    static size_t period_len = 0;       /* length of the period */

    if (!initialized) {
        const seq_t N = (seq_t)665;
        a0 = floor_sqrt(N);

        /* Compute the continued‑fraction expansion of sqrt(N) and
           store the periodic part in `period`.  The expansion terminates
           when the term equals 2*a0. */
        seq_t m = 0;
        seq_t d = 1;
        seq_t a_val = a0;
        const seq_t two_a0 = a0 + a0;

        while (1) {
            /* next (m, d, a) values */
            m = d * a_val - m;
            d = (N - m * m) / d;
            a_val = (a0 + m) / d;

            period[period_len++] = a_val;
            if (a_val == two_a0)
                break;
        }
        initialized = true;
    }

    if (n == 0) {
        return a0;
    }

    /* (n-1) because a(0) is the first term.  Use modulo to wrap
       within the period. */
    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}
