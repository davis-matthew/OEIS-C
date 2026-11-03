/*
 * A024564  a(n) = [ 1/{n*sqrt(7)} ], where {x} := x - [ x ].
 * https://oeis.org/A024564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(1/{n*sqrt(7)}) = floor( 2*floor_sqrt(7*n^2) /
 *            (7*n^2 - floor_sqrt(7*n^2)^2) )
 * Range:     u8 n <= 3, u16 n <= 48, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 111 terms through n = 860 (2026-11-01)
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
    if (n == 0)                 /* sequence offset starts at 1 */
        return 0;

    seq_t m = (seq_t)n;            /* cast index to seq_t   */
    seq_t M = (seq_t)7 * m * m;    /* M = 7*n^2              */
    seq_t s = floor_sqrt(M);       /* s = floor(sqrt(7*n^2))*/
    seq_t e = M - s * s;           /* e = 7*n^2 - s^2        */
    return ((seq_t)2 * s) / e;     /* floor(2s/e)            */
}
