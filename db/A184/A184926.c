/*
 * A184926  n+[rn/t]+[sn/t]+[un/t], where []=floor and r=1, s=sqrt(3),
 *          t=sqrt(5), u=sqrt(7).
 * https://oeis.org/A184926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(sqrt(n^2/5)) + floor(sqrt(3*n^2/5)) +
 *            floor(sqrt(7*n^2/5))
 * Range:     u8 n <= 6, u16 n <= 108, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Validated: partial, pass, 120 terms through n = 120 (2026-11-01)
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
    seq_t N = (seq_t)n;
    seq_t n2 = N * N;
    seq_t t1 = floor_sqrt(n2 / (seq_t)5);
    seq_t t2 = floor_sqrt(((seq_t)3 * n2) / (seq_t)5);
    seq_t t3 = floor_sqrt(((seq_t)7 * n2) / (seq_t)5);
    return N + t1 + t2 + t3;
}
