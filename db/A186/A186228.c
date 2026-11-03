/*
 * A186228  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f and g are the triangular numbers and
 *          heptagonal numbers. Complement of A186227.
 * https://oeis.org/A186228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = floor_T(H_n) + floor_H(H_n) * * where * H_n = n(5n-3)/2
 *            (the n‑th heptagonal number) * floor_T(x) = floor(
 *            (sqrt(8x+1)-1)/2 ) * floor_H(x) = floor( (3 + sqrt(9+40x))/10 )
 *            * * The sequence A186228 lists the rank of each heptagonal
 *            number * within the joint sorted list of triangular and he...
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Validated: partial, pass, 96 terms through n = 96 (2026-11-01)
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
    /* Compute the n-th heptagonal number H_n = n(5n-3)/2 */
    seq_t nn = (seq_t)n;
    seq_t h  = (nn * ((nn * 5) - 3)) / 2;

    /* Number of triangular numbers <= h:
     *   floor_T(h) = floor((sqrt(8h+1)-1)/2)   */
    seq_t t          = 8 * h + 1;
    seq_t sqrt_t     = floor_sqrt(t);
    seq_t tri_count  = (sqrt_t - 1) / 2;

    /* Number of heptagonal numbers <= h:
     *   floor_H(h) = floor((3 + sqrt(9+40h))/10) */
    seq_t rad         = 9 + 40 * h;
    seq_t sqrt_rad    = floor_sqrt(rad);
    seq_t hept_count  = (sqrt_rad + 3) / 10;

    /* Rank of the n-th heptagonal number = sum of counts */
    return tri_count + hept_count;
}
