/*
 * A185597  a(n) = floor(n^(3/2) - n^(1/2)); complement of A185598.
 * https://oeis.org/A185597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n^(3/2) – n^(1/2) ) = floor( (n‑1) * sqrt(n) )
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 96, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 86 terms through n = 4869 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This can be rewritten as a(n) = floor_sqrt( n * (n‑1)^2 ). */

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
    /* (n‑1)^2 * n fits in a 64‑bit unsigned integer for all n <= 10⁶,
       and any larger seq_t type can handle arbitrary precision. */
    seq_t m = (seq_t)(n - 1);      /* (n‑1) as seq_t */
    seq_t prod = m * m * (seq_t)n; /* (n‑1)^2 * n */
    return floor_sqrt(prod);
}
