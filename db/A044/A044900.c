/*
 * A044900  Base 14 run lengths alternate: even, odd, even,...
 * https://oeis.org/A044900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   alt_base14_runs_even_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑14 representations have run
   lengths alternating even, odd, even, ... starting with even.
   a(n) is the nth such integer. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/alt_base14_runs_even_odd.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t candidate = 1;

    while (1) {
        if (alt_base14_runs_even_odd(candidate)) {
            count++;
            if (count == n) return candidate;
        }
        candidate++;
    }
}
