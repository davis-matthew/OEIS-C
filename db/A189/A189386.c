/*
 * A189386  a(n) = n+[ns/r]+[nt/r]; r=1, s=sqrt(2), t=1/sqrt(3), []=floor.
 * https://oeis.org/A189386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sqrt(2)) + floor(n / sqrt(3)) which is
 *            equivalent to a(n) = n + floor_sqrt(2*n*n) + floor_sqrt(3*n*n) /
 *            3
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 99 terms through n = 4681 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Cast n to seq_t to avoid overflow when squaring on small types */
    seq_t ns = (seq_t)n;
    seq_t nsq = ns * ns;                     /* n^2 */
    seq_t part1 = floor_sqrt(nsq * (seq_t)2);   /* floor(sqrt(2*n^2)) = floor(n*sqrt(2)) */
    seq_t part2 = floor_sqrt(nsq * (seq_t)3) / (seq_t)3; /* floor(n/sqrt(3)) */
    return ns + part1 + part2;
}
