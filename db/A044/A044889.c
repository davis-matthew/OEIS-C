/*
 * A044889  Base-3 run lengths alternate: even, odd, even,...
 * https://oeis.org/A044889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   has_alternating_ternary_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044889 – Base‑3 run lengths alternate: even, odd, even,... */

/* The implementation scans integers and checks their ternary representation.
   It returns the nth term (1‑based offset 1). */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/has_alternating_ternary_runs.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t val = 1; ; ++val) {
        if (has_alternating_ternary_runs(val)) {
            ++count;
            if (count == n)
                return (seq_t)val;
        }
    }
}
