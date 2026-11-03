/*
 * A022805  a(n) = B(n) + C(n) where B(n) is Beatty sequence [ n*sqrt(3) ] and
 *          C is complement of B.
 * https://oeis.org/A022805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*sqrt(3)) + floor((3*n + floor(n*sqrt(3)))/2)
 * Range:     u8 n <= 4, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
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
    /* Compute B(n) = floor(n*sqrt(3)) as floor_sqrt(3*n*n)           */
    seq_t nn = (seq_t)n * (seq_t)n;               /* n² */
    seq_t b   = floor_sqrt((seq_t)3 * nn);        /* B(n) */

    /* Compute C(n) = floor((3*n + b)/2)                            */
    seq_t c   = ((seq_t)3 * (seq_t)n + b) / 2;    /* C(n) */

    return b + c;                                /* a(n) = B(n)+C(n) */
}
