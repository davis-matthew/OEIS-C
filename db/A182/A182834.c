/*
 * A182834  Complement of A007590, except for initial zeros.
 * https://oeis.org/A182834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(2*n-2))
 * Range:     u8 n <= 32, u16 n <= 8148, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9715 (2026-11-01)
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

/* Compute the n-th term of OEIS A182834.
 * The function is safe for any n ≥ 1, uses only integer arithmetic,
 * and works for all seq_t sizes supported by the build. */
seq_t a(idx_t n)
{
    /* Offset is 1; return 0 if a 0 index is mistakenly supplied. */
    if (n == 0)
        return (seq_t)0;

    /* Compute 2*n-2 in idx_t (uint64_t).  For all practical values of n
     * this will not overflow. */
    idx_t val = 2 * n - 2;

    /* floor_sqrt is provided by the utilities and returns a seq_t. */
    seq_t sq = floor_sqrt((seq_t)val);

    /* The result is n plus this integer square root. */
    return (seq_t)n + sq;
}
