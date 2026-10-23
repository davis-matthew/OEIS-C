/*
 * A043143  Numbers k such that 2 and 4 occur juxtaposed in the base-7
 *          representation of k but not of k-1.
 * https://oeis.org/A043143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   contains_adjacent_2_4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for k such that the base‑7 representation of k contains the
   digits 2 and 4 adjacent to each other (in either order) but
   the representation of k-1 does not. */

#include <stdbool.h>
#include "../utils/contains_adjacent_2_4.h"

seq_t a(idx_t n)
{
    seq_t k = 1;
    idx_t count = 0;

    while (1) {
        if (contains_adjacent_2_4(k) && !contains_adjacent_2_4(k - 1)) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}
