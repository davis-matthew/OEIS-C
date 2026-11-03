/*
 * A135675  a(n) = ceiling(n^(4/3) - n).
 * https://oeis.org/A135675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n^(4/3) - n)
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 112, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 963 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    /* cast index to the sequence type */
    seq_t n_seq = (seq_t)n;

    /* compute n^4; uses seq_t arithmetic which may be big-integer-enabled */
    seq_t n2  = n_seq * n_seq;
    seq_t n4  = n2 * n2;

    /* floor of n^(4/3) is floor((n^4)^(1/3)) */
    seq_t c   = floor_cube_root(n4);

    /* difference to subtract integer n */
    seq_t diff = c - n_seq;

    /* check if n is a perfect cube */
    seq_t t   = floor_cube_root(n_seq);
    seq_t t3  = t * t * t;
    if (t3 == n_seq)
        return diff;          /* n^(4/3) is integer, so ceil equals floor */
    else
        return diff + 1;      /* otherwise add one to get the ceiling */
}
