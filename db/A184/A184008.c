/*
 * A184008  n+floor(sqrt(7n/3)); complement of A184009.
 * https://oeis.org/A184008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(7n/3))
 * Range:     u8 n <= 27, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
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

/* Computes a(n) as defined for OEIS sequence A184008.
   Works with any seq_t and idx_t types provided by the build. */
seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;     /* cast index to seq_t for arithmetic */
    seq_t t = N * 7;        /* 7*n */
    seq_t div3 = t / 3;     /* integer division gives floor(7*n/3) */
    seq_t root = floor_sqrt(div3);  /* floor(sqrt(floor(7*n/3))) */
    return N + root;
}
