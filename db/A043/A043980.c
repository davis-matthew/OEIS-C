/*
 * A043980  Numbers k such that 2 and 8 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_adjacent28_82
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth integer k such that the base‑9 representation of k contains
 * the digits 2 and 8 adjacent (in either order) and the base‑9 representation
 * of k+1 does not contain adjacent 2 and 8. */

#include <stdbool.h>
#include "../utils/has_adjacent28_82.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1 */

    idx_t count = 0;
    seq_t k = 1;

    while (count < n) {
        if (has_adjacent28_82(k) && !has_adjacent28_82(k + 1)) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
    return k;   /* unreachable, keeps compiler happy */
}
