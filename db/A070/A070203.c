/*
 * A070203  Number of scalene integer triangles with perimeter n having
 *          integral inradius.
 * https://oeis.org/A070203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of scalene integer triangles with perimeter n
 *            having integer inradius
 * Range:     u8 n <= 23, u16 n <= 643, u32 n <= 4999, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 106 terms through n = 4890 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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
    if (n < 3) return 0;  /* no nondegenerate triangle possible */
    seq_t count = 0;
    for (idx_t a = 1; a <= n/3; ++a) {
        /* b must satisfy a < b < c and triangle inequality a + b > c. */
        idx_t b_start = a + 1;
        idx_t b_end   = (n - a - 1) / 2;  /* ensures b < c (since c = n - a - b)   */
        if (b_start > b_end) continue;
        for (idx_t b = b_start; b <= b_end; ++b) {
            idx_t c = n - a - b;
            if (a + b <= c) continue;      /* redundant safety check */
            /* All sides are distinct: a < b < c is guaranteed. */
            seq_t na = (seq_t)n - 2*(seq_t)a;
            seq_t nb = (seq_t)n - 2*(seq_t)b;
            seq_t nc = (seq_t)n - 2*(seq_t)c;
            seq_t numerator = na * nb;
            numerator *= nc;                          /* (n-2a)(n-2b)(n-2c)  */
            seq_t denom = 4 * (seq_t)n;               /* 4n  */
            if (numerator % denom != 0) continue;     /* r^2 not integer → skip */
            seq_t q = numerator / denom;             /* r^2 = q           */
            if (q == 0) continue;                    /* inradius cannot be 0 for a valid triangle */
            seq_t sq = floor_sqrt(q);                /* integer sqrt of q  */
            if (sq * sq == q) {                      /* q is a perfect square */
                ++count;
            }
        }
    }
    return count;
}
