/*
 * A043652  Numbers whose base-12 representation has exactly 3 runs.
 * https://oeis.org/A043652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has_three_runs_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based implementation of A043652:
   a(n) = n-th integer whose base-12 representation has exactly 3 runs   */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_three_runs_uint64.h"

/* External-function returning the n-th element of the sequence.
   idx_t is an unsigned integer type (uint64_t) supplied by the build,
   seq_t is an unsigned integer type or arbitrary-precision integer. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t x = 1;
    for (;;) {
        if (has_three_runs_uint64((uint64_t)x)) {
            ++count;
            if (count == n) return (seq_t)x;
        }
        ++x;
    }
}
