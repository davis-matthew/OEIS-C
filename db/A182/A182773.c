/*
 * A182773  Beatty sequence for 1+2^(2/3).
 * https://oeis.org/A182773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(1+2^(2/3)))
 * Range:     u8 n <= 2, u16 n <= 20, u32 n <= 392, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9900 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the identity
   a(n) = n + floor((4*n^3)^(1/3)),
   where (4*n^3)^(1/3) is the integer cube root of 4*n^3.
   The floor_cube_root utility handles arbitrary-precision integers,
   so the same code works for all supported seq_t definitions. */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Convert n to seq_t */
    seq_t ns = (seq_t)n;

    /* Compute 4 * n^3  */
    seq_t n3 = ns * ns * ns;      /* n^3  */
    seq_t val = n3 * 4;           /* 4 * n^3 */

    /* floor((4*n^3)^(1/3)) */
    seq_t m = floor_cube_root(val);

    /* a(n) = n + m */
    return ns + m;
}
