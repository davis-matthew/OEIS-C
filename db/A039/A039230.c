/*
 * A039230  Numbers whose base-12 representation has the same number of 2's
 *          and 3's.
 * https://oeis.org/A039230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   equal_23
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/equal_23.h"

/* Search: enumerates integers and keeps those whose base‑12 representation
   contains an equal number of 2s and 3s. */

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t current = (seq_t)0;
    while (true) {
        if (equal_23(current)) {
            found++;
            if (found == n)
                return current;
        }
        current = current + (seq_t)1;
    }
}
