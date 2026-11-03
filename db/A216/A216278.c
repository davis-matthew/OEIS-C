/*
 * A216278  Number of solutions to the equation x^2 + 2*y^2 = n with x and y >
 *          0.
 * https://oeis.org/A216278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{ (x, y) | x>0, y>0, x^2 + 2*y^2 = n }
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 137 terms through n = 9549 (2026-11-01)
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
    /* Convert index to the sequence type for all arithmetic. */
    seq_t nn = (seq_t)n;
    seq_t count = 0;

    /* Search over y >= 1.  We stop when 2*y^2 >= n, as larger y
       would give a negative remainder. */
    for (idx_t y = 1; ; ++y)
    {
        seq_t yseq = (seq_t)y;
        seq_t y2   = yseq * yseq;   /* y^2 */
        seq_t two_y2 = y2 + y2;     /* 2*y^2 */

        if (two_y2 >= nn)
            break;                  /* no more admissible y */

        seq_t rem = nn - two_y2;    /* remaining part for x^2 */

        seq_t x = floor_sqrt(rem);  /* integer sqrt of rem */

        /* x must be positive and square exactly. */
        if (x > 0 && x * x == rem)
            ++count;
    }

    return count;
}
