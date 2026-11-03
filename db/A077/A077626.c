/*
 * A077626  Largest term in periodic part of continued fraction expansion of
 *          square root of 1+3^n or 0 if 1+3^n is square.
 * https://oeis.org/A077626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if 1+3^n is a perfect square, otherwise 2 *
 *            floor(sqrt(1+3^n))
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-01)
 * Helpers:   ipow_i32, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/ipow_idx.h"

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
    /* Compute 3^n */
    seq_t power = ipow_idx((seq_t)3, n);

    /* D = 1 + 3^n */
    seq_t D = power + (seq_t)1;

    /* floor(sqrt(D)) */
    seq_t a0 = floor_sqrt(D);

    /* Check if D is a perfect square */
    if (a0 * a0 == D)
        return (seq_t)0;

    /* Largest term in periodic part of continued fraction of sqrt(D) */
    return a0 * (seq_t)2;
}
