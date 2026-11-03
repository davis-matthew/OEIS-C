/*
 * A023973  First bit in fractional part of binary expansion of 6th root of n.
 * https://oeis.org/A023973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(2*(n^(1/6) - floor(n^(1/6))))
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Validated: partial, pass, 102 terms through n = 102 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is 0 if the first binary digit after the point of n^(1/6) is 0,
   1 otherwise. */
#include <stdint.h>

/* compute x^6 using unsigned __int128 (x fits in uint64_t). */
static inline unsigned __int128 pow6_uint64(uint64_t x)
{
    unsigned __int128 y = x;
    y = y * y;           /* x^2   */
    unsigned __int128 y2 = y * y; /* x^4   */
    return y2 * y;       /* x^6   */
}

/* integer sixth root: floor(n^(1/6)).  n is idx_t (uint64_t). */
static inline uint64_t int_root6(idx_t n)
{
    /* for n ≤ 2^64-1, the sixth root never exceeds 2048 (2^11). */
    uint64_t low = 0, high = 2048;
    while (low + 1 < high) {
        uint64_t mid = low + (high - low) / 2;
        if (pow6_uint64(mid) <= (unsigned __int128)n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* main sequence function. */
seq_t a(idx_t n)
{
    uint64_t k = int_root6(n);                     /* floor(n^(1/6))          */
    unsigned __int128 left  = (unsigned __int128)n * 64;          /* 64*n   */
    unsigned __int128 right = pow6_uint64(2 * k + 1);             /* (2k+1)^6 */
    return (left >= right) ? (seq_t)1 : (seq_t)0;
}
