/*
 * A010237  Continued fraction for sqrt(199).
 * https://oeis.org/A010237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   continued fraction for sqrt(199). a(0) = floor(sqrt(199)) for n
 *            >= 1, a(n) returns the (n‑1)th element of the repeating period
 *            of the continued fraction of sqrt(199). The period has length
 *            20: 9,2,1,2,2,5,4,1,1,13,1,1,4,5,2,2,1,2,9,28
 * Range:     u8 none, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Generic type definitions for seq_t and idx_t are provided by the build environment. */

seq_t a(idx_t n)
{
    /* static cache of the periodic part of the expansion */
    static bool initialized = false;
    static seq_t period[64];        /* ample space for period */
    static size_t period_len = 0;

    if (!initialized)
    {
        const seq_t D   = 199;
        const seq_t a0  = floor_sqrt(D);
        seq_t m = 0;
        seq_t d = 1;
        seq_t a_curr = a0;

        size_t pos = 0;
        do
        {
            /* next triple (m,d,a_curr) */
            m = d * a_curr - m;
            d = (D - m * m) / d;
            a_curr = (a0 + m) / d;
            period[pos++] = a_curr;            /* store the periodic term */
        } while (a_curr != a0 * 2);            /* period ends when a_curr = 2*a0 */

        period_len = pos;
        initialized = true;
    }

    if (n == 0)
        return floor_sqrt(199);

    /* n >= 1 : return the (n-1)th term of the period   */
    idx_t idx = (n - 1) % period_len;
    return period[(size_t)idx];
}
