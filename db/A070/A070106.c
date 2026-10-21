/*
 * A070106  Number of integer triangles with perimeter n which are obtuse and
 *          isosceles.
 * https://oeis.org/A070106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max(0, n - ceil(n*sqrt(2)/2) - ⌊n/4⌋)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where ceil(n*sqrt(2)/2) = ⌊⌊sqrt(2*n^2)⌋/2⌋ + 1. */
/* This counts the integer isosceles obtuse triangles with perimeter n. */

#include <stdint.h>

/* Integer square root of a 128‑bit value: floor(sqrt(v)) */
static uint64_t isqrt_u128(unsigned __int128 v)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1) << 64; /* hi^2 fits in 128 bits */
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return (uint64_t)lo;
}

seq_t a(idx_t n)
{
    /* n is 1‑based, n ≥ 1 */
    unsigned __int128 two_n2 = (unsigned __int128)n * n * 2;          /* 2*n^2 */
    uint64_t sqrt_floor = isqrt_u128(two_n2);                         /* floor(sqrt(2*n^2)) */
    uint64_t ceil_half = (sqrt_floor >> 1) + 1;                       /* ceil(n*sqrt(2)/2) */
    uint64_t n_div4 = n / 4;                                          /* floor(n/4) */
    int64_t val = (int64_t)n - (int64_t)ceil_half - (int64_t)n_div4; /* n - ceil(...) - floor(n/4) */
    if (val < 0)
        return 0;
    return (seq_t)val;
}
