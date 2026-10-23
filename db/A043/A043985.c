/*
 * A043985  Numbers k such that 3 and 8 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer k such that the base‑9 digits of k
 *            contain the digits 3 and 8 next to each other (in either order),
 *            but the base‑9 digits of k + 1 do not
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_adjacent_38_or_83
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_adjacent_38_or_83.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n == 0, harmless value        */
    idx_t found = 0;
    seq_t k = 1;
    for (;;) {
        if (has_adjacent_38_or_83(k)) {
            seq_t next = k + 1;
            if (!has_adjacent_38_or_83(next)) {
                ++found;
                if (found == n)
                    return k;
            }
        }
        ++k;
    }
}
