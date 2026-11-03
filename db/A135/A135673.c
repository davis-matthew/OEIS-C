/*
 * A135673  Ceiling(n + n^(2/3)).
 * https://oeis.org/A135673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n + n^(2/3))
 * Range:     u8 n <= 7, u16 n <= 164, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 82 terms through n = 926 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
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
    if (n == 0) return 0;                    /* offset is 1, but guard */
    seq_t nseq = (seq_t)n;
    seq_t n2   = nseq * nseq;                /* n^2 */
    seq_t r    = floor_cube_root(n2);        /* floor(n^(2/3)) */
    seq_t c    = floor_cube_root(nseq);      /* cube root of n */
    seq_t cube = c * c * c;                  /* c^3 */
    bool perfect = (cube == nseq);           /* n is a perfect cube? */
    seq_t res = nseq + r + (perfect ? 0 : 1);
    return res;
}
