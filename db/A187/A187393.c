/*
 * A187393  a(n) = floor(r*n), where r = 4 + sqrt(8); complement of A187394.
 * https://oeis.org/A187393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((4 + sqrt(8)) * n) = 4*n + floor(2*n*sqrt(2)) which
 *            equals 4*n + floor(sqrt(8*n^2))
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-01)
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
    if (n == 0)  // undefined for offset 1, but guard
        return 0;
    seq_t tn = (seq_t)n;
    seq_t sq = tn * tn;          /* n^2          */
    sq *= (seq_t)8;              /* 8*n^2        */
    seq_t s = floor_sqrt(sq);    /* floor(sqrt(8*n^2)) */
    return tn * (seq_t)4 + s;    /* 4*n + s */
}
