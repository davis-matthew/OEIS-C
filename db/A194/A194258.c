/*
 * A194258  Second inverse function (numbers of columns) for pairing function
 *          A060734.
 * https://oeis.org/A194258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = min{t; n - (t - 1)^2} with t = floor(sqrt(n-1)) + 1
 * Range:     u8 n <= 9870, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root for 64‑bit unsigned integers, returns floor(sqrt(x)) */
static inline idx_t _floor_isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = ((idx_t)1) << 62;           /* highest power of four <= 2^64 */
    while (bit > x)
        bit >>= 2;
    while (bit != 0)
    {
        if (x >= r + bit)
        {
            x -= r + bit;
            r = (r >> 1) + bit;
        }
        else
            r >>= 1;
        bit >>= 2;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for n < 1, return 0 */
        return (seq_t)0;

    idx_t k = _floor_isqrt_u64(n - 1);
    seq_t t    = (seq_t)(k + 1);
    seq_t diff = (seq_t)n - (seq_t)k * (seq_t)k;
    return (t < diff) ? t : diff;
}
