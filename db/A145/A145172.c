/*
 * A145172  Number of pentagonal numbers needed to represent n with greedy
 *          algorithm.
 * https://oeis.org/A145172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pentagonal numbers needed to represent n * by
 *            the greedy algorithm (largest possible pentagonal subtracted
 *            first)
 * Range:     u8 n <= 2, u16 n <= 671, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 9624 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: pentagonal number P(k) = k*(3k-1)/2 for k >= 1. */
static inline seq_t pentagonal(seq_t k)
{
    return k * (3 * k - 1) / 2;
}

/* Required utility for integer square root of a seq_t. */
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

/* Main OEIS function. */
seq_t a(idx_t n)
{
    seq_t rem   = (seq_t)n;   /* remaining value to represent */
    seq_t count = 0;          /* number of pentagonal numbers used */

    while (rem > 0)
    {
        /* Compute upper bound k for which P(k) <= rem. */
        seq_t d = 1 + 24 * rem;         /* argument for sqrt */
        seq_t sqrt_d = floor_sqrt(d);   /* integer sqrt of d */
        idx_t k = (idx_t)((1 + sqrt_d) / 6); /* floor((1+sqrt_d)/6) */

        /* Adjust k downward until P(k) <= rem. */
        while (pentagonal((seq_t)k) > rem)
            --k;

        rem   -= pentagonal((seq_t)k);
        count += 1;
    }

    return count;
}
