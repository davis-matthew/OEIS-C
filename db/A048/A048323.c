/*
 * A048323  Numbers whose base-12 expansions, read from left to right, have
 *          run lengths that strictly decrease.
 * https://oeis.org/A048323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th non-negative integer whose base‑12 expansion has
 *            strictly decreasing run lengths
 * Range:     u8 n <= 34, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   has_decreasing_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_decreasing_runs.h"

seq_t a(idx_t n)
{
    idx_t seen = 0;
    seq_t val = 0;

    while (1) {
        if (has_decreasing_runs(val)) {
            if (seen == n) return val;
            seen++;
        }
        val++;
    }
}
