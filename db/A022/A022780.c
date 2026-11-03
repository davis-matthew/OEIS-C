/*
 * A022780  Place where n-th 1 occurs in A023118.
 * https://oeis.org/A022780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 55, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 830 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + 1 + ⌊(n-1)/√5⌋,   a(1) = 1
 *
 * The recurrence follows from the definition
 *     a(n) = n + 1 + Σ_{k=0}^{n} ⌊(n-k)/√5⌋
 * and the identity  ⌊(n-1)/√5⌋ = ⌊⌊(n-1)·√5⌋/5⌋
 * (computed exactly via integer arithmetic).
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
    if (n <= 1) return (seq_t)1;   /* a(1) = 1 */

    seq_t result = (seq_t)1;        /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        idx_t t = i - 1;                       /* t = i-1 */
        seq_t val = (seq_t)t;                  /* convert to seq_t */
        seq_t tmp = 5 * val * val;             /* 5·t²  (exact in seq_t) */
        seq_t sqrt_val = floor_sqrt(tmp);      /* ⌊t·√5⌋ = ⌊√(5t²)⌋ */
        seq_t floor_div = sqrt_val / 5;        /* ⌊t/√5⌋  */
        seq_t inc = floor_div + 1;             /* 1 + ⌊t/√5⌋   */
        result += inc;
    }
    return result;
}
