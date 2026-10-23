/*
 * A043148  Numbers k such that 3 and 6 occur juxtaposed in the base-7
 *          representation of k but not of k-1.
 * https://oeis.org/A043148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has_adjacent_36_or_63
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/has_adjacent_36_or_63.h"

/* a(n): the n‑th integer k whose base‑7 representation contains the
 * adjacent digits 3 and 6 in either order, while k-1 does not. */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* out of range, shouldn't happen */

    seq_t k = 1;
    idx_t count = 0;

    while (true) {
        if (has_adjacent_36_or_63(k) && !has_adjacent_36_or_63(k - 1)) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}
