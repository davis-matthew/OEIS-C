/*
 * A043924  Numbers k such that 2 and 5 occur juxtaposed in the base-7
 *          representation of k but not of k+1.
 * https://oeis.org/A043924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has_adjacent_2_5_or_5_2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043924: numbers k such that the base‑7 representation of k
   contains the adjacent digits 2 and 5 (either order) but the
   representation of k+1 does not.  Computed by a straightforward
   sequential search. */

#include <stdbool.h>
#include "../utils/has_adjacent_2_5_or_5_2.h"

seq_t a(idx_t n) {
    /*  n is 1‑based;  n ≥ 1 */
    idx_t count = 0;
    seq_t k = (seq_t)1;
    for (;;) {
        if (has_adjacent_2_5_or_5_2(k) && !has_adjacent_2_5_or_5_2(k + (seq_t)1)) {
            ++count;
            if (count == n) return k;
        }
        ++k;
    }
}
