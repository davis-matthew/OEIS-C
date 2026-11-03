/*
 * A087374  Smallest square >= n!.
 * https://oeis.org/A087374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(sqrt(n!))^2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

static seq_t fact_upto(idx_t m)
{
    seq_t r = 1;
    for (idx_t i = 2; i <= m; ++i)
        r *= (seq_t)i;
    return r;
}

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
    /* Compute n! */
    seq_t fact = fact_upto(n);

    /* Compute floor(sqrt(n!)) */
    seq_t root = floor_sqrt(fact);

    /* Square it */
    seq_t root_sq = root * root;

    /* If it's smaller than n!, increment root and square again */
    if (root_sq < fact) {
        root_sq = (root + 1) * (root + 1);
    }

    return root_sq;
}
