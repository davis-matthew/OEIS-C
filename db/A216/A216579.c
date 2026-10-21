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
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumerates all b > 0 and checks whether a^2 = n – 10·b^2 is a positive square. */
#include "../utils/floor_sqrt.h"

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
