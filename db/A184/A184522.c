/*
 * A184522  Upper s-Wythoff sequence, where s=5n. Complement of A184523.
 * https://oeis.org/A184522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n/2)*(-3+sqrt(29)))
 * Range:     u8 n <= 1, u16 n <= 23, u32 n <= 120, u64 n <= 120,
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

seq_t a(idx_t n) {
    /* Compute ⌊n*sqrt(29)⌋ using integer square root */
    seq_t n_sq   = (seq_t)n * n;          /* n^2 */
    seq_t val    = n_sq * (seq_t)29;      /* 29*n^2  */
    seq_t t      = floor_sqrt(val);       /* ⌊n*sqrt(29)⌋ = ⌊sqrt(29*n^2)⌋ */

    /* a(n) = ⌊(⌊n*sqrt(29)⌋ - 3n)/2⌋  */
    seq_t three_n = (seq_t)n * (seq_t)3;  /* 3n */
    seq_t diff    = t - three_n;         /* ⌊n*sqrt(29)⌋ – 3n */
    return diff / 2;                     /* integer division truncates the fraction */
}
