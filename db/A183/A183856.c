/*
 * A183856  n+floor(sqrt(3n-2)); complement of A143975.
 * https://oeis.org/A183856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(3*n-2))
 * Range:     u8 n <= 21, u16 n <= 5166, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 9617 (2026-11-01)
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
    /* n is an unsigned 64‑bit integer, cast to seq_t for arithmetic */
    seq_t NN = (seq_t)n;
    seq_t t   = 3 * NN - (seq_t)2;        /* argument of the square root */
    seq_t sq  = floor_sqrt(t);            /* floor(sqrt(t)) */
    return NN + sq;                       /* a(n) = n + floor(sqrt(3n-2)) */
}
