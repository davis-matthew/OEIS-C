/*
 * A123860  a(n) = ceiling( (7 + sqrt(49+24*n))/2 ).
 * https://oeis.org/A123860
 *
 * Offset:    -2    (a(-2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil( (7 + sqrt(49+24*n)) / 2 )
 * Range:     u8 n <= 0, u16 n <= 669, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 90 terms through n = 862 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
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
    /* Compute S = 49 + 24*n as a signed 64‑bit integer
       (n ≥ –2 guarantees S ≥ 1). */
    idx_t S_signed = 49 + 24 * n;

    /* Convert to the build's integer type. */
    seq_t S = (seq_t)S_signed;

    /* Integer (floor) square root of S. */
    seq_t sqrt_floor = floor_sqrt(S);

    /* Test whether S is a perfect square. */
    seq_t sqrt_sq = sqrt_floor * sqrt_floor;
    bool perfect = (sqrt_sq == S);

    /* Base = 7 + floor(sqrt(S)). */
    seq_t base = (seq_t)7 + sqrt_floor;

    seq_t result;
    if (perfect) {
        /* if sqrt is integer, ceil((7+sqrt)/2) = (base+1)/2 */
        result = (base + 1) / 2;
    } else {
        /* if sqrt is not integer, ceil = floor(base/2) + 1 */
        result = (base / 2) + 1;
    }

    return result;
}
