/*
 * A184812  n+floor(ns/r)+floor(nt/r), where r=sqrt(2), s=sqrt(3), t=sqrt(5).
 * https://oeis.org/A184812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sqrt(3/2)) + floor(n*sqrt(5/2))
 * Range:     u8 n <= 5, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 8279 (2026-11-01)
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

/* Computes the a(n) term for OEIS A184812.
   Uses only integer arithmetic, no floating‑point operations. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below offset, behaviour is irrelevant */

    /* work in seq_t to avoid overflow for large seq_t builds    */
    seq_t n_t  = (seq_t)n;          /* convert index to seq_t  */
    seq_t n_sq = n_t * n_t;         /* n^2                       */

    /* terms for sqrt(3/2) and sqrt(5/2)                           */
    seq_t m3 = n_sq * (seq_t)3;     /* 3*n^2                      */
    seq_t m5 = n_sq * (seq_t)5;     /* 5*n^2                      */

    /* floor(m / 2)  (integer division truncates)                  */
    seq_t b3 = m3 / 2;
    seq_t b5 = m5 / 2;

    /* floor_sqrt returns ⌊√x⌋                                         */
    seq_t f3 = floor_sqrt(b3);     /* ⌊√(3*n^2 / 2)⌋ = ⌊n*√(3/2)⌋   */
    seq_t f5 = floor_sqrt(b5);     /* ⌊√(5*n^2 / 2)⌋ = ⌊n*√(5/2)⌋   */

    return n_t + f3 + f5;
}
