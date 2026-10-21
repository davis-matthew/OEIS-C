/*
 * A054791  Earliest sequence with a(a(n)) = n^2.
 * https://oeis.org/A054791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nice, nonn
 * Formula:   if n is a square then a(n) = a(sqrt(n))^2; otherwise if n -
 *            floor(sqrt(n))^2 is odd then a(n) = n+1; else a(n) = (n-1)^2
 * Range:     u8 n <= 17, u16 n <= 257, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 64‑bit unsigned integer, floor(sqrt(x)). */
static inline uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t low = 0, high = 0xFFFFFFFFULL;  /* sqrt(2^64-1) == 2^32-1 */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;     /* round up */
        if (mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n <= 1)                    /* base cases: a(0)=0, a(1)=1 */
        return (seq_t)n;

    uint64_t r = isqrt_uint64((uint64_t)n);
    seq_t r_sq = (seq_t)r * (seq_t)r;

    if (r_sq == (seq_t)n) {        /* n is a perfect square */
        seq_t val = a(r);          /* recurse on sqrt(n) */
        return val * val;
    } else {                       /* n is not a perfect square */
        seq_t diff = (seq_t)n - r_sq;   /* n - floor(sqrt(n))^2 */
        if (diff & 1U)                /* odd difference -> n+1 */
            return (seq_t)n + 1;
        else                          /* even difference -> (n-1)^2 */
        {
            seq_t t = (seq_t)n - 1;
            return t * t;
        }
    }
}
