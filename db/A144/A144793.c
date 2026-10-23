/*
 * A144793  Consider the runs of 0's in the binary representation of n, each
 *          of these runs being on the edge of the binary representation n
 *          and/or being bounded by 1's. Consider also the runs of 1's in the
 *          binary representation of n, each of these runs being on the edge
 *          of the binary representation n and/or...
 * https://oeis.org/A144793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   equal_shortest_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n-th positive integer whose shortest runs of 0's
   and shortest runs of 1's in its binary representation are equal. */
#include <stdbool.h>
#include <limits.h>
#include "../utils/equal_shortest_runs.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* not used, but safe */
    idx_t found = 0;
    seq_t candidate = 1;
    while (found < n) {
        if (equal_shortest_runs(candidate)) {
            found++;
            if (found == n) return candidate;
        }
        candidate++;
    }
    return candidate;  /* unreachable, keeps compiler happy */
}
