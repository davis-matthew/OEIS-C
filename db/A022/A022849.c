/*
 * A022849  Integer nearest nx, where x = sqrt(6).
 * https://oeis.org/A022849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nearest integer to n*sqrt(6)
 * Range:     u8 n <= 3, u16 n <= 52, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9974 (2026-11-01)
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
    seq_t nn  = (seq_t)n;                 /* cast index to seq_t */
    seq_t sq  = (seq_t)6 * nn * nn;       /* 6*n^2       */
    seq_t s   = floor_sqrt(sq);           /* floor(√(6*n²)) */

    /* Determine if round up is needed:
       left  = 24*n²
       right = 4*s² + 4*s + 1
       If left ≥ right → ceil, else floor.      */
    seq_t left   = (seq_t)24 * nn * nn;
    seq_t right  = (seq_t)4 * s * s + (seq_t)4 * s + (seq_t)1;

    if (left >= right)
        return s + 1;
    else
        return s;
}
