/*
 * A038350  Numbers whose base-6 representation has the same nonzero number of
 *          1's and 4's.
 * https://oeis.org/A038350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   nth number whose base‑6 representation has the same * positive
 *            number of 1's and 4's. That is, in base‑6 the counts of * digits
 *            1 and 4 are equal and both > 0. a(1) = 10, a(2) = 25, …
 * Range:     u8 n <= 32, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   has_balanced_ones_and_fours
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/has_balanced_ones_and_fours.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1 – value 0 is undefined */
    idx_t found = 0;
    seq_t cand = 1;
    for (;;) {
        if (has_balanced_ones_and_fours(cand)) {
            ++found;
            if (found == n) return cand;
        }
        ++cand;
    }
}
