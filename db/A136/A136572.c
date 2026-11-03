/*
 * A136572  Triangle read by rows: row n consists of n zeros followed by n!.
 * https://oeis.org/A136572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r! if n = r(r+3)/2 for some integer r≥0; otherwise a(n)=0
 * Range:     u8 n <= 6, u16 n <= 53, u32 n <= 103, u64 n <= 251,
 *            u128 n <= 664, big any n
 * Validated: standard, pass, 94 terms through n = 5124 (2026-11-01)
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

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    /* Compute 8*n + 9 in seq_t to avoid overflow on large n. */
    seq_t t = ((seq_t)n) * 8 + 9;
    /* Integer square root of t. */
    seq_t s = floor_sqrt(t);
    /* Check perfect square condition. */
    if (s * s != t)
        return 0;
    /* s must be at least 3; otherwise r would be negative. */
    if (s < 3)
        return 0;
    /* Compute r = (s - 3)/2.  s-3 must be even. */
    seq_t diff = s - 3;
    if ((diff & 1) != 0)      /* odd -> no integer r */
        return 0;
    seq_t r = diff >> 1;      /* r ≥ 0 */
    /* For safety, verify that r(r+3)/2 equals n (prevents overflow errors). */
    idx_t check = (idx_t)r * ((idx_t)r + 3) / 2;
    if (check != n)
        return 0;
    /* Return factorial of r. */
    return factorial((idx_t)r);
}
