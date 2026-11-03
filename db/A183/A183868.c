/*
 * A183868  a(n) = n + floor(2*sqrt(n+1)); complement of A079524.
 * https://oeis.org/A183868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(2*sqrt(n+1))
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-01)
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
    /* The OEIS offset is 1; for n < 1 the result is not defined. */
    if (n < 1u) return (seq_t)0;

    seq_t n_se = (seq_t)n;
    seq_t m     = n_se + 1;          /* n + 1 */
    seq_t s     = floor_sqrt(m);     /* floor(sqrt(n+1)) */

    /* term = n + 2*floor(sqrt(n+1)) or +1 more if the fraction part ≥ 0.5 */
    seq_t term = n_se + 2 * s;

    seq_t s_sqr = s * s;
    seq_t upper = s_sqr + s;         /* s^2 + s  (threshold for +1) */

    if (m > upper)
        term += 1;

    return term;
}
