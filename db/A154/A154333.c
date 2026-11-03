/*
 * A154333  Difference between n^3 and the next smaller square.
 * https://oeis.org/A154333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^3 - floor(sqrt(n^3 - 1))^2
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 77 terms through n = 940 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the difference between n^3 and the next smaller perfect square. */

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
    /* Convert the index to the sequence type to avoid intermediate overflow */
    seq_t x = (seq_t) n;

    /* Compute n^3 */
    seq_t n3 = x * x * x;

    /* Compute floor(sqrt(n^3 - 1)) */
    seq_t sqroot = floor_sqrt(n3 - (seq_t)1);

    /* Square the root to get the largest square < n^3 */
    seq_t sq = sqroot * sqroot;

    /* The desired difference */
    return n3 - sq;
}
