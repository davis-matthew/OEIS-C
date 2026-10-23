/*
 * A043213  Numbers k such that 6 and 7 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_adjacent_67or76
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the search for the n‑th number k
 * whose base‑9 representation contains the adjacent digits
 * 6 and 7 (in either order) while the representation of k‑1
 * does not.  It iterates k in increasing order, counting
 * qualifying values until the desired index n is reached. */
#include <stdbool.h>
#include "../utils/has_adjacent_67or76.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (seq_t k = 1; ; ++k) {
        if (has_adjacent_67or76(k)) {
            if (!has_adjacent_67or76(k - 1)) {
                ++count;
                if (count == n)
                    return k;
            }
        }
    }
}
