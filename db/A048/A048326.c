/*
 * A048326  Numbers whose base-15 expansions, read from left to right, have
 *          run lengths that strictly decrease.
 * https://oeis.org/A048326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers whose base‑15 expansions, read from left to right, have
 *            run lengths that strictly decrease
 * Range:     u8 n <= 43, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   decreasing_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/decreasing_runs.h"

/* Return the nth term of OEIS A048326 (offset 0). */
seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t candidate = (seq_t)0;

    while (true) {
        if (decreasing_runs(candidate)) {
            if (count == n)
                return candidate;
            ++count;
        }
        ++candidate;
    }
}
