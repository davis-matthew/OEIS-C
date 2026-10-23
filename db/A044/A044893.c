/*
 * A044893  Base 7 run lengths alternate: even, odd, even,...
 * https://oeis.org/A044893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   a044893_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044893:
   a(n) is the n‑th positive integer whose base‑7 representation has
   run‑lengths alternating in parity, starting with even.
   (e.g. 8 → 11₇, runs 2 even; 58 → 112₇, runs 2 even, 1 odd, etc.) */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/a044893_valid.h"

/* Return the nth term of the sequence (n ≥ 1).  If n < 1, 0 is returned. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    idx_t count = 0;
    for (idx_t x = 8; ; ++x) {          /* start at the first possible term */
        if (a044893_valid(x)) {
            ++count;
            if (count == n)
                return (seq_t)x;
        }
    }
}
