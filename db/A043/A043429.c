/*
 * A043429  Numbers having one 2 in base 8.
 * https://oeis.org/A043429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer whose base‑8 representation
 *            contains exactly one digit 2
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   has_one_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_one_two.h"

/* Public interface defined by the build system. */
seq_t a(idx_t n)
{
    idx_t  seen = 0;           /* how many qualifying numbers we have found so far */
    seq_t  candidate = 1;      /* start checking from 2 */

    while (true) {
        candidate++;                      /* next integer to test */
        if (has_one_two(candidate)) {
            ++seen;
            if (seen == n)
                return candidate;        /* nth qualifying number reached */
        }
    }
}
