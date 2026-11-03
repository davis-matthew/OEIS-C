/*
 * A131431  3n + 1 preceded by n zeros.
 * https://oeis.org/A131431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 3r+1 if n = r(r+3)/2 for some r >= 0; otherwise 0
 * Range:     u8 n <= 6, u16 n <= 788, u32 n <= 11324, u64 n <= 11324,
 *            u128 n <= 11324, big any n
 * Validated: standard, pass, 107 terms through n = 9808 (2026-11-01)
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
    /* Compute discriminant D = 9 + 8n */
    seq_t D = (seq_t)8 * n + 9;
    /* Compute integer sqrt of D */
    seq_t s = floor_sqrt(D);
    /* If D is not a perfect square, a(n) is 0 */
    if (s * s != D)
        return 0;

    /* Now check if s-3 is an even non‑negative number */
    if (s < 3)
        return 0;
    seq_t diff = s - 3;
    if (diff & 1)
        return 0;  /* not divisible by 2 */

    seq_t r = diff >> 1;   /* r = (s-3)/2 */
    return 3 * r + 1;
}
