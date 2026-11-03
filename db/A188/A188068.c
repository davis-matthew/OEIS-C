/*
 * A188068  a(n) = [n*r]-[k*r]-[n*r-k*r], where r=sqrt(3), k=1, [ ]=floor.
 * https://oeis.org/A188068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*sqrt(3)) – floor(sqrt(3)) – floor((n–1)*sqrt(3))
 *            Since floor(sqrt(3)) = 1, this simplifies to a(n) =
 *            floor(n*sqrt(3)) – 1 – floor((n–1)*sqrt(3)). The function
 *            computes a(n) by integer arithmetic using the built‑in
 *            floor_sqrt routine
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 139 terms through n = 8831 (2026-11-01)
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
    /* floor(n*sqrt(3)) = floor(sqrt(3*n^2)) */
    seq_t nn   = (seq_t)n;
    seq_t m1   = nn * nn * (seq_t)3;
    seq_t f1   = floor_sqrt(m1);

    /* floor((n-1)*sqrt(3)) = floor(sqrt(3*(n-1)^2)) */
    idx_t n1i  = n - (idx_t)1;
    seq_t n1   = (seq_t)n1i;
    seq_t m2   = n1 * n1 * (seq_t)3;
    seq_t f2   = floor_sqrt(m2);

    /* a(n) = f1 - 1 - f2 */
    return f1 - (seq_t)1 - f2;
}
