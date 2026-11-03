/*
 * A216579  Number of positive integer solutions to the equation a^2 + 10*b^2
 *          = n.
 * https://oeis.org/A216579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 107 terms through n = 9613 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumerates all b > 0 and checks whether a^2 = n – 10·b^2 is a positive square. */
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
    /* Convert the index to seq_t for arithmetic. */
    seq_t n_seq = (seq_t)n;
    seq_t count = 0;

    /* At most b^2 ≤ n/10, so compare as seq_t. */
    seq_t limit = n_seq / 10;

    for (idx_t b = 1; ; ++b)
    {
        seq_t bb = (seq_t)b * (seq_t)b;
        if (bb > limit)            /* 10·b^2 would exceed n */
            break;

        seq_t rem = n_seq - (seq_t)10 * bb;
        if (rem == 0)              /* a would be 0, not positive */
            continue;

        seq_t sqrt_rem = floor_sqrt(rem);
        if (sqrt_rem * sqrt_rem == rem && sqrt_rem > 0)
            ++count;
    }

    return count;
}
