/*
 * A180122  First of three "least, sum, least" self-generating sequences.
 * https://oeis.org/A180122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(1+sqrt(3))) - 1
 * Range:     u8 n <= 4, u16 n <= 62, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9458 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements sequence A180122 */
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

/* Returns the nth term of the sequence, with a(1) = 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n = 0, return 0 as a safe default */

    seq_t ns = (seq_t)n;
    /* Compute floor(n*sqrt(3)) as floor(sqrt(3*n*n)) */
    seq_t t = ns * ns * (seq_t)3;
    seq_t sq = floor_sqrt(t);

    /* a(n) = floor(n*sqrt(3)) + n - 1 */
    return sq + ns - (seq_t)1;
}
