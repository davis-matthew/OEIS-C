/*
 * A197702  Smallest positive integer k such that n = +-1 +-3 +-... +-(2k-1)
 *          for some choice of +'s and -'s.
 * https://oeis.org/A197702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Let a = floor(sqrt(n)). If a*a == n, return a. Else let d
 *            = (2*a + 1) - (n - a*a). (d = ceil_sqrt(n)^2 - n) If d == 4,
 *            return a + 3; else if d is odd, return a + 2; else return a + 1;
 *            (This implements the OEIS conjecture validity test.)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
static idx_t floor_sqrt_u64(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = ((idx_t)1 << 32);          /* 2^32 is > sqrt(2^64-1) */
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid > 0 && mid > n / mid) {   /* mid*mid > n  */
            hi = mid - 1;
        } else {                          /* mid*mid <= n  */
            lo = mid + 1;
        }
    }
    return hi;                            /* hi is floor(sqrt(n)) */
}

seq_t a(idx_t n)
{
    /* OEIS offset starts at 1; n >= 1 for valid calls */
    idx_t a_floor = floor_sqrt_u64(n);
    if ((uint64_t)a_floor * (uint64_t)a_floor == (uint64_t)n) {
        return (seq_t)a_floor;
    }
    /* n is not a perfect square */
    idx_t a = a_floor;
    idx_t a2 = a * a;                     /* a*a fits in 64 bits */
    idx_t diff = n - a2;                  /* 0 < diff < 2a+1   */
    idx_t d = (2 * a + 1) - diff;          /* ceil_sqrt(n)^2 - n */
    idx_t k = a + 1;                       /* ceil_sqrt(n)      */
    if (d == 4) {
        return (seq_t)(k + 2);
    } else if (d & 1) {
        return (seq_t)(k + 1);
    } else {
        return (seq_t)k;
    }
}
