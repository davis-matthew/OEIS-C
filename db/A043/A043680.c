/*
 * A043680  Numbers whose base-16 representation has exactly 7 runs.
 * https://oeis.org/A043680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   hex_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/hex_runs.h"
/* This function enumerates integers in increasing order and returns the nth integer whose base‑16 representation has exactly 7 runs. */

seq_t a(idx_t n)
{
    /* The smallest number with 7 runs in hex is 0x1010101. */
    const seq_t start = (seq_t)0x1010101;
    seq_t cur = start;
    idx_t cnt = 0;

    for (;;) {
        if (hex_runs(cur) == 7) {
            ++cnt;
            if (cnt == n)
                return cur;
        }
        ++cur;
    }
}
