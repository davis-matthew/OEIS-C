/*
 * A182769  Beatty sequence for (4 + sqrt(2))/2.
 * https://oeis.org/A182769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(4 + sqrt(2))/2)
 * Range:     u8 n <= 5, u16 n <= 65, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9915 (2026-11-01)
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
    /* Convert index to seq_t for arithmetic. */
    seq_t N = (seq_t)n;

    /* Compute t = 2 * N * N and its integer square root.
       floor(sqrt(2 * N * N)) equals floor(N * sqrt(2)). */
    seq_t t = 2 * N * N;
    seq_t s = floor_sqrt(t);

    /* The desired term: 2*N + floor(s/2). */
    return 2 * N + s / 2;
}
