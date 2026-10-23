/*
 * A120883  (1/4)*number of lattice points with odd indices in a square
 *          lattice inside a circle around the origin with radius 2*n.
 * https://oeis.org/A120883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} ceil( sqrt(n^2 - (i - 1/2)^2) - 1/2 )
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer implementation: a(n) = Σ_{i=1..n} ( floor( sqrt((2n)^2 - (2i-1)^2) ) + 1 ) / 2 */

#include <stdint.h>   /* for uint64_t */

static inline idx_t isqrt_u64(idx_t v)
{
    /* integer floor square root for 0 <= v < 2^64 */
    idx_t r = 0;
    idx_t bit = ((idx_t)1) << 62;          /* highest power of four ≤ 2^64-1 */

    while (bit > v)
        bit >>= 2;

    while (bit != 0)
    {
        if (v >= r + bit)
        {
            v -= r + bit;
            r = (r >> 1) + bit;
        }
        else
        {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum = 0;

    /* (2n)^2 fits comfortably into idx_t for the intended ranges */
    idx_t two_n = 2 * n;
    idx_t four_n_sq = two_n * two_n;

    for (idx_t i = 1; i <= n; ++i)
    {
        idx_t odd_x = 2 * i - 1;          /* 2i-1  */
        idx_t x_sq  = odd_x * odd_x;      /* (2i-1)^2 */

        idx_t t = four_n_sq - x_sq;       /* (2n)^2 - (2i-1)^2 */

        /* floor sqrt of t */
        idx_t sqrt_t = isqrt_u64(t);

        /* number of admissible odd y-values for this x */
        seq_t count = (seq_t)(sqrt_t + 1) / 2;

        sum += count;
    }

    return sum;
}
