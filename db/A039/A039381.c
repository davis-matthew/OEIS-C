/*
 * A039381  Numbers whose base-9 representation has the same nonzero number of
 *          3's and 7's.
 * https://oeis.org/A039381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7662, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_equal_nonzero_37
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-9 representation has the same nonzero number of 3's and 7's.
   Implementation: brute-force search counting the 3's and 7's in each number. */

#include <stdbool.h>
#include "../utils/has_equal_nonzero_37.h"

/* Returns the nth member of A039381 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* offset starts at 1; 0 is unused */
    idx_t found = 0;
    for (seq_t x = 1; ; ++x) {
        if (has_equal_nonzero_37(x)) {
            ++found;
            if (found == n)
                return x;
        }
    }
}
