/*
 * A063725  Number of ordered pairs (x,y) of positive integers such that x^2 +
 *          y^2 = n.
 * https://oeis.org/A063725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (x , y) of positive integers such
 *            that x^2 + y^2 = n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 109 terms through n = 9617 (2026-11-01)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    /* n = 0 or 1: no positive integer solutions */
    if (n <= 1)
        return (seq_t)0;

    seq_t count = 0;
    /* maximum possible value of x is floor(sqrt(n-1)) */
    idx_t maxx = isqrt(n - 1);

    for (idx_t x = 1; x <= maxx; ++x)
    {
        /* compute x^2 as idx_t (safe because x <= sqrt(n-1)) */
        idx_t xsq = (idx_t)((seq_t)x * (seq_t)x);
        idx_t rem = n - xsq;          /* remaining value for y^2 */
        idx_t y   = isqrt(rem);

        if (y > 0 && (idx_t)((seq_t)y * (seq_t)y) == rem)
            ++count;
    }

    return count;
}
