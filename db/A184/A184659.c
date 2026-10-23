/*
 * A184659  floor(n*s+h-h*s), where s=(3+sqrt(5))/2, h=-1/3; complement of
 *          A184658.
 * https://oeis.org/A184659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*s + h – h*s), s = (3 + sqrt5)/2, h = -1/3
 *            Equivalent integer formula: let t = 3*n + 1 a(n) = floor( (
 *            floor(t*sqrt5) + 3*t – 2 ) / 6 ) where floor(t*sqrt5) is
 *            computed as floor( sqrt(5*t*t) )
 * Range:     u8 n <= 2, u16 n <= 37, u32 n <= 115, u64 n <= 115,
 *            u128 n <= 115, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root: floor(sqrt(x)) for any unsigned seq_t */
static seq_t isqrt_seq(seq_t x)
{
    if (x == 0 || x == 1)
        return x;

    seq_t low  = 0;
    seq_t high = 1;

    /* exponential search for high: high > sqrt(x) */
    while (high <= x / high)
        high <<= 1;

    low = high >> 1;

    /* binary search between low and high */
    while (low + 1 < high) {
        seq_t mid = (low + high) >> 1;
        if (mid <= x / mid)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* OEIS sequence A184659 */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset safety */
        return 0;

    /* t = 3*n + 1 (carry out in idx_t to avoid overflow of n) */
    idx_t t_idx = 3 * n + 1;
    seq_t t = (seq_t)t_idx;

    /* s = floor(t * sqrt5)  = floor( sqrt(5 * t * t) ) */
    seq_t tt   = t * t;
    seq_t val  = (seq_t)5 * tt;     /* 5 * t^2  */
    seq_t s    = isqrt_seq(val);   /* floor(sqrt(val)) */

    /* a(n) = floor( (s + 3*t – 2) / 6 ) */
    seq_t numerator = s + (seq_t)3 * t - (seq_t)2;
    seq_t result    = numerator / (seq_t)6;

    return result;
}
