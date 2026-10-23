/*
 * A131253  Row sums of triangle A131252.
 * https://oeis.org/A131253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 39, u64 n <= 84, u128 n <= 174,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A131253: Row sums of triangle A131252
   Recurrence (for n ≥ 6):
       a(n) = 4*a(n-1) - 4*a(n-2) - 2*a(n-3) + 4*a(n-4) - a(n-6)
   Initial values:
       a(0)=1, a(1)=3, a(2)=8, a(3)=17, a(4)=34, a(5)=64
   The implementation uses only the recurrence, computing the terms
   sequentially from 0 up to the requested n.  No dynamic allocation
   is required, so it works uniformly for unsigned integer and
   arbitrary‑precision types.  It respects the required interface and
   works in both C11 and C++17. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* constant initial terms */
    static const seq_t init[6] = {1, 3, 8, 17, 34, 64};

    if (n < 6)
        return init[(idx_t)n];

    /* hold the last 6 values needed for the recurrence */
    seq_t buf[6];
    for (idx_t i = 0; i < 6; ++i)
        buf[i] = init[i];

    for (idx_t i = 6; i <= n; ++i)
    {
        /* indexes of the needed previous terms */
        idx_t i1 = (i - 1) % 6;   /* a(n-1) */
        idx_t i2 = (i - 2) % 6;   /* a(n-2) */
        idx_t i3 = (i - 3) % 6;   /* a(n-3) */
        idx_t i4 = (i - 4) % 6;   /* a(n-4) */
        idx_t i6 = (i - 6) % 6;   /* a(n-6) */

        seq_t val = buf[i1] * (seq_t)4;
        val -= buf[i2] * (seq_t)4;
        val -= buf[i3] * (seq_t)2;
        val += buf[i4] * (seq_t)4;
        val -= buf[i6];

        buf[i % 6] = val;
    }

    return buf[n % 6];
}
