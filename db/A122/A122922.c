/*
 * A122922  Express n as the sum of four squares, x^2+y^2+z^2+w^2,
 *          x>=y>=z>=w>=0, minimizing the value of x, then minimizing y for
 *          that x. a(n) is that y.
 * https://oeis.org/A122922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed as the minimal y in a representation n = x² +
 *            y² + z² + w² with x ≥ y ≥ z ≥ w ≥ 0, minimizing x first and then
 *            minimizing y
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation note: The algorithm enumerates candidates
   in increasing order of x (and for each x, in increasing order
   of y). It stops as soon as a valid representation is found.  */

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

static inline idx_t isqrt_idx(idx_t n)
{
    /* Integer square root for 64‑bit values.  */
    idx_t r = (idx_t) sqrtl((long double) n);
    if (r > n) r = n;
    while ((r + 1) > 0 && (r + 1) * (r + 1) <= n) ++r;
    while (r * r > n) --r;
    return r;
}

seq_t a(idx_t n)
{
    idx_t max_x = isqrt_idx(n);
    /* Enumerate x from 0 upward (minimal x). */
    for (idx_t x = 0; x <= max_x; ++x)
    {
        idx_t rem = n - x * x;
        idx_t max_y = isqrt_idx(rem);
        if (max_y > x)   /* respect order x ≥ y */
            max_y = x;
        for (idx_t y = 0; y <= max_y; ++y)
        {
            idx_t t = rem - y * y;   /* remaining sum to be split into z² + w² */
            idx_t max_z = isqrt_idx(t);
            if (max_z > y)   /* respect order y ≥ z */
                max_z = y;
            for (idx_t z = 0; z <= max_z; ++z)
            {
                idx_t w2 = t - z * z;   /* w² = t - z² */
                if (w2 > z * z)        /* ensure w ≤ z */
                    continue;
                idx_t w = isqrt_idx(w2);
                if (w * w == w2 && w <= z)   /* valid representation found */
                    return (seq_t) y;
            }
        }
    }
    /* By Lagrange's theorem a representation always exists, so we never reach here. */
    return (seq_t) 0;
}
