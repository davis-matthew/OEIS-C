/*
 * A053411  Circle numbers (version 1): a(n)= number of points (i,j), i,j
 *          integers, contained in a circle of diameter n, centered at the
 *          origin.
 * https://oeis.org/A053411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + 2 * sum_{x=1..⌊n/2⌋} (2*⌊sqrt((n²‑4x²)/4)⌋ + 1)
 * Range:     u8 n <= 18, u16 n <= 288, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This counts the lattice points (i,j) inside a circle of diameter n.   */

#include <stdint.h>
#include <math.h>

/* integer square root for uint64_t, returned as seq_t */
static seq_t
isqrt_u64(uint64_t v)
{
    uint64_t r = (uint64_t)sqrt((long double)v);
    /* adjust down */
    while ((unsigned __int128)r * r > v) --r;
    /* adjust up */
    while ((unsigned __int128)(r + 1) * (r + 1) <= v) ++r;
    return (seq_t)r;
}

seq_t
a(idx_t n)
{
    uint64_t N4 = (uint64_t)n * (uint64_t)n;          /* n² */
    uint64_t half = n / 2;                           /* ⌊n/2⌋ */
    seq_t sum = 0;

    for (uint64_t x = 0; x <= half; ++x) {
        uint64_t t = 4 * x * x;                      /* 4x² */
        if (t > N4) break;
        uint64_t rhs = (N4 - t) / 4;                /* floor((N4‑4x²)/4) */
        seq_t ymax = isqrt_u64(rhs);                /* max |y| for this x */
        seq_t cnt_y = ymax * 2 + 1;                 /* number of y for this x */

        if (x == 0)
            sum += cnt_y;
        else
            sum += cnt_y * 2;                       /* ±x */
    }

    return sum;
}
