/*
 * A210535  Second inverse function (numbers of columns) for pairing function
 *          A209293.
 * https://oeis.org/A210535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9870, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A210535 (Second inverse function for pairing function A209293)
 * Formula: a(n) = 2*i - v*(4*i - 2*t - 3)
 *   where
 *     t = ⌊(√(8*n−7)−1)/2⌋,
 *     i = n − t*(t+1)/2,
 *     v = ⌊(2*n+1 − t*(t+1))/(t+3)⌋
 * This implementation uses only integer arithmetic and a binary‑search
 * integer square root to avoid floating‑point operations.
 */

#include <stdint.h>

/* 128‑bit integer square root: returns ⌊√x⌋  (x fits in signed __int128) */
static inline unsigned long long isqrt_u128(__int128 x)
{
    unsigned long long lo = 0;
    /* 8*n-7 < 8*2^64 = 2^67, so √x < 2^34.  Use a small upper bound. */
    unsigned long long hi = (1ULL << 34);
    while (lo < hi) {
        unsigned long long mid = (lo + hi + 1) >> 1;
        __int128 mid2 = (__int128)mid * mid;
        if (mid2 <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* the sequence is defined for n ≥ 1; return 0 for n = 0 */
        return (seq_t)0;

    __int128 n128 = n;

    /* Compute t */
    __int128 temp = 8 * n128 - 7;
    unsigned long long t = (isqrt_u128(temp) - 1) / 2;
    __int128 t128 = t;

    /* i = n − t*(t+1)/2  */
    __int128 Tt = t128 * (t128 + 1) / 2;
    __int128 i = n128 - Tt;

    /* v = ⌊(2*n+1 − t*(t+1))/(t+3)⌋  */
    __int128 numerator = 2 * n128 + 1 - t128 * (t128 + 1);
    __int128 v = numerator / (t128 + 3);

    /* a(n) = 2*i − v*(4*i − 2*t − 3)  */
    __int128 term2 = 4 * i - 2 * t128 - 3;
    __int128 res   = 2 * i - v * term2;

    return (seq_t)res;
}
