/*
 * A039389  Numbers whose base-9 representation has the same nonzero number of
 *          5's and 8's.
 * https://oeis.org/A039389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers with equal non‑zero numbers of 5’s and 8’s in base 9
 * Range:     u8 n <= 6, u16 n <= 7663, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_equal_58
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_equal_58.h"

seq_t a(idx_t n)
{
    seq_t x = 1;          /* start checking from 1 */
    idx_t found = 0;      /* how many valid numbers seen */

    for (;; ++x) {
        if (has_equal_58(x)) {
            ++found;
            if (found == n)
                return x;
        }
    }
}
