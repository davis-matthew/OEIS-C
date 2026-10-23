/*
 * A044901  Base 15 run lengths alternate: even, odd, even,...
 * https://oeis.org/A044901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer whose base‑15 expansion has run
 *            lengths that alternate even, odd, even, … (the first run must be
 *            even)
 * Range:     u8 n <= 28, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   base15_alternating_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/base15_alternating_runs.h"

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* undefined, but safe fallback */

    seq_t cur = 1;
    idx_t count = 0;

    while (1) {
        if (base15_alternating_runs(cur)) {
            ++count;
            if (count == n)
                return cur;
        }
        ++cur;
    }
}
