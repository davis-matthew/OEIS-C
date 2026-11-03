/*
 * A135676  a(n) = floor(n^(4/3) - n).
 * https://oeis.org/A135676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^(4/3) - n)
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 139, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 932 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) = floor_cube_root(n^4) - n. */
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

seq_t a(idx_t n) {
    /* For n >= 1 the result is guaranteed to be non‑negative and fit in seq_t. */
    seq_t nseq = (seq_t)n;            /* convert index to seq_t */
    seq_t n2   = nseq * nseq;         /* n^2 */
    seq_t n4   = n2 * n2;             /* n^4 */
    seq_t root = floor_cube_root(n4); /* floor(n^(4/3)) */
    return root - nseq;               /* floor(n^(4/3)) - n == floor(n^(4/3) - n) */
}
