/*
 * A044961  Numbers with no two equally numerous base 12 digits.
 * https://oeis.org/A044961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   distinct_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Series: numbers with distinct base-12 digit counts */

#include <stdbool.h>
#include "../utils/distinct_counts.h"

seq_t a(idx_t n)
{
    idx_t seen = 0;
    seq_t cand = 1;
    while (1) {
        if (distinct_counts(cand)) {
            ++seen;
            if (seen == n)
                return cand;
        }
        ++cand;
    }
}
