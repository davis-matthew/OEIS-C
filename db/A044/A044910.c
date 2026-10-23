/*
 * A044910  Numbers whose base-9 run lengths alternate: odd, even, odd, ...
 * https://oeis.org/A044910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   valid_base9_alt_runlen
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044910 – numbers whose base‑9 run lengths alternate odd, even, odd, ….
   Formula: a(n) = nth integer (n ≥ 1) whose base–9 digits form runs of alternating
   parity, starting with an odd run. This implementation searches sequentially. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/valid_base9_alt_runlen.h"

/* Return the n-th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* safety for non‑positive indices */

    idx_t counted = 0;
    seq_t cand = 0;

    while (counted < n) {
        ++cand;
        if (valid_base9_alt_runlen(cand))
            ++counted;
    }
    return cand;
}
