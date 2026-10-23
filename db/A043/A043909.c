/*
 * A043909  Numbers k such that 3 and 5 occur juxtaposed in the base-6
 *          representation of k but not of k+1.
 * https://oeis.org/A043909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   has_35or53
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043909: numbers k such that 3 and 5 occur juxtaposed (in either order)
   in the base‑6 representation of k but not of k+1.
   This implementation searches sequentially for the n‑th qualifying value. */

#include <stdbool.h>
#include "../utils/has_35or53.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; values are requested for n >= 1. */
    if (n == (idx_t)0)
        return (seq_t)0;           /* undefined for offset 1 */

    seq_t k = (seq_t)1;
    idx_t found = (idx_t)0;

    for (;;) {
        if (has_35or53(k) && !has_35or53(k + (seq_t)1)) {
            ++found;
            if (found == n)
                return k;
        }
        ++k;
    }
}
