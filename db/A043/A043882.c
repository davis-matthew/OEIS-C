/*
 * A043882  Numbers k such that 0 and 3 occur juxtaposed in the base-4
 *          representation of k but not of k+1.
 * https://oeis.org/A043882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the nth integer k such that
   the base‑4 representation of k contains an adjacent pair
   of digits 0 and 3 (either 03 or 30) and the base‑4
   representation of k+1 does *not* contain such a pair. */

#include <stdbool.h>
#include <stdint.h>

static bool has_adjacent_03_or_30(idx_t v)
{
    /* Return true iff the base‑4 representation of v contains
       a 0 and a 3 in consecutive positions (00:0-3 or 3-0). */
    if (v < 4) return false;
    idx_t prev = v % 4;
    v /= 4;
    while (v > 0) {
        idx_t cur = v % 4;
        if ((prev == 0 && cur == 3) || (prev == 3 && cur == 0))
            return true;
        prev = cur;
        v /= 4;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 0; ; ++k) {
        if (has_adjacent_03_or_30(k) && !has_adjacent_03_or_30(k + 1)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
