/*
 * A143434  Expansion of f(x, -x^3) in powers of x where f(,) is Ramanujan's
 *          two-variable theta function.
 * https://oeis.org/A143434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 0 if n is not a triangular number; otherwise a(n) =
 *            (-1)^floor((k+2)/4) where k satisfies n = k*(k+1)/2
 * Range:     i8 n <= 7, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 121 terms through n = 880 (2026-11-01)
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
    /* compute 8*n + 1 in seq_t */
    seq_t term = (seq_t)n * 8 + 1;

    /* integer square root */
    seq_t s = floor_sqrt(term);

    /* candidate k */
    seq_t k = (s - 1) / 2;

    /* check triangularity: T_k = k*(k+1)/2 */
    seq_t T = k * (k + 1) / 2;
    if (T != (seq_t)n)
        return 0;

    /* sign = (-1)^floor((k+2)/4) */
    seq_t exp = (k + 2) / 4;
    return (exp & 1) ? -1 : 1;
}
