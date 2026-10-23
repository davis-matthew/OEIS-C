/*
 * A043892  Numbers k such that 1 and 4 occur juxtaposed in the base-5
 *          representation of k but not of k+1.
 * https://oeis.org/A043892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   has_adjacent14
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: the n‑th integer k whose base‑5 representation contains an
   adjacent pair of digits 1 and 4 (in either order) while k+1 does not. */
#include <stdbool.h>
#include "../utils/has_adjacent14.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    idx_t count = 0;
    seq_t k = 1;

    while (1) {
        if (has_adjacent14(k) && !has_adjacent14(k + 1)) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}
