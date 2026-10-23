/*
 * A023532  a(n) = 0 if n is of the form m*(m+3)/2, otherwise 1.
 * https://oeis.org/A023532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if 8*n+9 is a perfect square (equivalently, n =
 *            m*(m+3)/2 for some m), otherwise 1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for unsigned __int128 values.  Returns floor(sqrt(x)). */
static inline uint64_t isqrt_u128(unsigned __int128 x)
{
    uint64_t lo = 0;
    /* The maximum needed sqrt is < 2^34 for all 0 ≤ n < 2^64 */
    uint64_t hi = (uint64_t)1 << 34;
    uint64_t mid, r = 0;

    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            lo = mid + 1;
            r = mid;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return r;          /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    /* Compute 8*n+9 in 128‑bit to avoid overflow for large n. */
    unsigned __int128 val = (unsigned __int128)8 * (unsigned __int128)n + 9;
    uint64_t sq = isqrt_u128(val);
    if ((unsigned __int128)sq * sq == val)
        return (seq_t)0;
    else
        return (seq_t)1;
}
