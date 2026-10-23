/*
 * A183301  Complement of A014105.
 * https://oeis.org/A183301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt((n-1)/2))
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of an unsigned 64‑bit integer.
   Returns floor(sqrt(x)) using only integer arithmetic. */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = 1ULL << 32;          /* 2^32 > sqrt(UINT64_MAX) */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        /* mid*mid <= x   <=>   mid <= x/mid  (avoids overflow) */
        if (mid > 0 && mid > x / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety for offset 1 */
    uint64_t tmp = (n - 1) / 2;
    uint64_t s   = isqrt_u64(tmp);
    return (seq_t)n + (seq_t)s;
}
