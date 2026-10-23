/*
 * A043951  Numbers k such that 3 and 5 occur juxtaposed in the base-8
 *          representation of k but not of k+1.
 * https://oeis.org/A043951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   has_adj_35
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043951: numbers k such that 3 and 5 occur juxtaposed in
 * the base‑8 representation of k but not in the base‑8 representation
 * of k+1. */
#include <stdbool.h>
#include "../utils/has_adj_35.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    for (seq_t k = 1; ; ++k) {
        if (has_adj_35(k)) {
            seq_t kp = k + 1;
            if (!has_adj_35(kp)) {
                ++found;
                if ((idx_t)found == n)
                    return k;
            }
        }
    }
}
