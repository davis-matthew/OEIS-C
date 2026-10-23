/*
 * A031969  Numbers with exactly four distinct base-10 digits.
 * https://oeis.org/A031969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th positive integer whose base‑10 representation
 *            contains exactly four distinct digits
 * Range:     u8 none, u16 n <= 4536, u32 n <= 4536, u64 n <= 4536,
 *            u128 n <= 4536, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_counts.h"

static bool has_four_distinct(seq_t x)
{
    unsigned char cnt[10] = {0};
    digit_counts(x, cnt);
    idx_t distinct = 0;
    for (int d = 0; d < 10; ++d)
        if (cnt[d])
            ++distinct;
    return distinct == 4;
}

seq_t a(idx_t n)
{
    /* Start from the first number known to have four distinct digits. */
    seq_t x = 1023;
    idx_t found = 0;

    while (found < n)
    {
        if (has_four_distinct(x))
        {
            ++found;
            if (found == n)
                return x;
        }
        ++x;
    }

    /* Should never reach here. */
    return 0;
}
