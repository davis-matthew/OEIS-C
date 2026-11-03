/*
 * A218173  Expansion of f(x^7, x^17) - x^2 * f(x, x^23) in powers of x where
 *          f(,) is Ramanujan's two-variable theta function.
 * https://oeis.org/A218173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (exists integer k: n = 12k^2 + 5k) - (exists integer k: n
 *            = 12k^2 + 11k + 2) Equivalently: let D = 25 + 48n and s =
 *            floor(sqrt(D)). If s*s != D then a(n)=0. If s ≡ 5 or 19 (mod 24)
 *            then a(n)= 1. If s ≡ 11 or 13 (mod 24) then a(n)= -1
 * Range:     i8 n <= 0, i16 n <= 330, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 101 terms through n = 996 (2026-11-01)
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

seq_t a(idx_t n) {
    /* Compute D = 25 + 48*n using seq_t to avoid overflow   */
    seq_t D = (seq_t)48 * (seq_t)n + (seq_t)25;
    seq_t s = floor_sqrt(D);
    if (s * s != D) return (seq_t)0;

    unsigned r = (unsigned)(s % (seq_t)24);    /* s is non‑negative */

    switch (r) {
        case 5:
        case 19:  return (seq_t)1;
        case 11:
        case 13:  return (seq_t)-1;
        default:  return (seq_t)0;
    }
}
