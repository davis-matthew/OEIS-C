/*
 * A043717  Numbers whose base-14 representation has an odd number of runs.
 * https://oeis.org/A043717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose base‑14 representation has
 *            an odd number of runs. Computed by linear search
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_odd_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_odd_runs.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for n<1, but safe return */

    idx_t found = 0;
    seq_t candidate = 1;

    while (found < n) {
        if (has_odd_runs(candidate)) {
            ++found;
            if (found == n)
                return candidate;
        }
        ++candidate;
    }
    return candidate; /* unreachable */
}
