/*
 * A039344  Numbers whose base-8 representation has the same nonzero number of
 *          2's and 5's.
 * https://oeis.org/A039344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   has_equal_nonzero_25
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: brute‑force search for the nth integer whose base‑8
   representation has equal nonzero counts of digit 2 and digit 5,
   and both counts are > 0.  This satisfies OEIS A039344. */
#include <stdbool.h>
#include "../utils/has_equal_nonzero_25.h"

/* sequence function required by the build */
seq_t a(idx_t n)
{
    seq_t val = 0;
    idx_t count = 0;
    while (count < n) {
        val += 1;
        if (has_equal_nonzero_25(val))
            count += 1;
    }
    return val;
}
