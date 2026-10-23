/*
 * A043111  Numbers k such that 1 and 3 occur juxtaposed in the base-5
 *          representation of k but not of k-1.
 * https://oeis.org/A043111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   has_adjacent_1_3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that 1 and 3 occur juxtaposed in the base‑5 representation of k
   (either order: “13” or “31”) but not of k‑1. */
#include <stdbool.h>
#include "../utils/has_adjacent_1_3.h"

seq_t a(idx_t n)
{
    seq_t k = 0;           /* current candidate */
    idx_t count = 0;       /* how many terms found so far */

    while (count < n) {
        ++k;
        if (has_adjacent_1_3(k) && !has_adjacent_1_3(k - 1))
            ++count;
    }
    return k;
}
