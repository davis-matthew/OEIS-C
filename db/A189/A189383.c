/*
 * A189383  a(n) = n + [n*s/r] + [n*t/r]; r=1, s=1/sqrt(2), t=1/sqrt(3).
 * https://oeis.org/A189383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n/√2) + floor(n/√3)
 * Range:     u8 n <= 11, u16 n <= 258, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 100 terms through n = 956 (2026-11-01)
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
    /* Convert the index to seq_t */
    seq_t N = (seq_t)n;

    /* Compute N² */
    seq_t Nsq = N * N;

    /* floor(sqrt(N² / 2)) == floor(N / √2) */
    seq_t part1 = floor_sqrt(Nsq / 2);

    /* floor(sqrt(N² / 3)) == floor(N / √3) */
    seq_t part2 = floor_sqrt(Nsq / 3);

    return N + part1 + part2;
}
