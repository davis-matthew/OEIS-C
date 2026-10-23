/*
 * A033717  Number of integer solutions to the equation x^2 + 2*y^2 + 4*z^2 =
 *          n.
 * https://oeis.org/A033717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the number of integer solutions of x^2 + 2*y^2 + 4*z^2 = n. */
#include <stdint.h>
#include <stdbool.h>

/* Integer square‑root of an unsigned 64‑bit value. */
static idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = 1ULL << 31;          /* highest power of two ≤ sqrt(2^64) */
    while (bit)
    {
        idx_t trial = r | bit;
        if (trial <= x / trial)      /* avoid overflow */
            r = trial;
        bit >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t cnt = 0;

    /* iterate over non‑negative z. */
    idx_t maxz = isqrt_u64(n / 4);
    for (idx_t z = 0; z <= maxz; ++z)
    {
        idx_t z2   = z * z;
        idx_t t    = n - 4 * z2;          /* remaining value after z */

        /* iterate over non‑negative y. */
        idx_t maxy = isqrt_u64(t / 2);
        for (idx_t y = 0; y <= maxy; ++y)
        {
            idx_t y2 = y * y;
            idx_t rem = t - 2 * y2;       /* remaining value for x^2 */

            idx_t sq = isqrt_u64(rem);
            if (sq * sq == rem)          /* rem is a perfect square */
            {
                int signz = (z == 0) ? 1 : 2;
                int signy = (y == 0) ? 1 : 2;
                int signx = (sq == 0) ? 1 : 2;
                cnt += (seq_t)(signz * signy * signx);
            }
        }
    }

    return cnt;
}
