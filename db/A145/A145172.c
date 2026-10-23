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
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: pentagonal number P(k) = k*(3k-1)/2 for k >= 1. */
static inline seq_t pentagonal(seq_t k)
{
    return k * (3 * k - 1) / 2;
}

/* Required utility for integer square root of a seq_t. */
#include "../utils/floor_sqrt.h"

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
