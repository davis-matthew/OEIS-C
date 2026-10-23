/*
 * A048322  Numbers whose base-11 expansions, read from left to right, have
 *          run lengths that strictly decrease.
 * https://oeis.org/A048322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   dec_run_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/dec_run_ok.h"

/* Sequence A048322: numbers whose base‑11 expansions have strictly decreasing run
   lengths.  The function searches integers sequentially and returns the n‑th term. */

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t cur = 0;
    while (1) {
        if (dec_run_ok(cur)) {
            if (count == n)
                return cur;
            ++count;
        }
        ++cur;
    }
}
