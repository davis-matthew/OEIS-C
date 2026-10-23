/*
 * A184813  n+[nr/s]+[nt/s], where r=sqrt(2), s=sqrt(3), t=sqrt(5), and
 *          []=floor.
 * https://oeis.org/A184813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sqrt(2)/sqrt(3)) + floor(n*sqrt(5)/sqrt(3))
 * Range:     u8 n <= 12, u16 n <= 345, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent integer computation: a(n) = n + ⌊sqrt( (n²*2)/3 )⌋ + ⌊sqrt( (n²*5)/3 )⌋ */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for any unsigned integral type seq_t. */
static inline seq_t isqrt_seq(seq_t x)
{
    seq_t lo = 0;
    seq_t hi = x;
    while (lo <= hi)
    {
        seq_t mid = lo + ((hi - lo) >> 1);
        /* avoid overflow: mid*mid <= x  <=>  mid <= x/mid */
        if (mid == 0 || mid <= x / mid)
        {
            lo = mid + 1;
        }
        else
        {
            hi = mid - 1;
        }
    }
    return hi; /* hi is floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term1 = isqrt_seq(((nn * nn * (seq_t)2) / (seq_t)3));
    seq_t term2 = isqrt_seq(((nn * nn * (seq_t)5) / (seq_t)3));
    return nn + term1 + term2;
}
