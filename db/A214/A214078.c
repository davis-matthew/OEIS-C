/*
 * A214078  a(n) = (ceiling (sqrt(n)))!.
 * https://oeis.org/A214078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (ceil (sqrt(n)))!
 * Range:     u8 n <= 25, u16 n <= 64, u32 n <= 144, u64 n <= 400,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer floor square root for 64‑bit unsigned integers, 
   using only integer arithmetic */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low  = 0;
    /* sqrt(2^64-1) < 2^32, so high can be 2^32 */
    idx_t high = ((idx_t)1 << 32);   /* exclusive upper bound */

    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        /* mid*mid <= n  <=>  mid <= n/mid   (mid > 0) */
        if (mid != 0 && mid <= n / mid)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* factorial applied to an unsigned integer value */
static inline seq_t factorial_u64(idx_t m)
{
    seq_t r = 1;
    for (idx_t i = 2; i <= m; ++i)
        r *= (seq_t)i;
    return r;
}

seq_t a(idx_t n)
{
    idx_t floor_sqrt = isqrt_u64(n);
    idx_t ceil_sqrt  = (floor_sqrt * floor_sqrt == n) ? floor_sqrt : floor_sqrt + 1;
    return factorial_u64(ceil_sqrt);
}
