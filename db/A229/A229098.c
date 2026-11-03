/*
 * A229098  Smallest area A of Heron triangles with sides (a, b, c) in
 *          arithmetic progression of the form b - d(n), b, b + d(n), where
 *          d(n) = A091998(n) = 12*n +- 1.
 * https://oeis.org/A229098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest area of a Heron triangle with sides (b-d(n), b,
 *            b+d(n)), where d(n) = 1 for n=1, and for n>1: if n is even then
 *            d(n)=12*(n/2)-1, else d(n)=12*(n/2)+1
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-01)
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

/* helper to compute d(n) */
static seq_t d_of_n(idx_t n)
{
    if (n == 1)
        return 1;
    seq_t k = n / 2;           /* integer division truncates */
    if ((n & 1) == 0)          /* n even */
        return (seq_t)12 * k - 1;
    else                       /* n odd   */
        return (seq_t)12 * k + 1;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                /* out of range, return 0 */
        return 0;

    seq_t d  = d_of_n(n);      /* d(n) */
    seq_t start = 2 * d + 1;   /* minimal b satisfying b > 2d */

    for (seq_t b = start;; ++b)
    {
        seq_t t = b * b - 4 * d * d;   /* b^2 - 4 d^2 */

        if (t % 3 != 0)               /* must be divisible by 3 */
            continue;

        seq_t v2 = t / 3;             /* (b^2 - 4 d^2)/3 */
        seq_t v = floor_sqrt(v2);     /* integer square root */

        if (v * v != v2)              /* must be a perfect square */
            continue;

        if ((b * v) % 4 != 0)         /* area must be integer */
            continue;

        seq_t area = (3 * b * v) / 4; /* (3·b·v)/4, division exact */
        return area;
    }
}
