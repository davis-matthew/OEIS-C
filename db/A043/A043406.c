/*
 * A043406  Numbers whose base-7 representation contains exactly two 3's.
 * https://oeis.org/A043406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   has_two_threes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n-th integer whose base‑7 representation has exactly
   two digits equal to 3. */

#include <stdbool.h>
#include "../utils/has_two_threes.h"

seq_t a(idx_t n)
{
    seq_t candidate = 0;
    idx_t found = 0;

    /* Scan integers in increasing order until we have found n of them. */
    while (found < n) {
        if (has_two_threes(candidate)) {
            found++;
            if (found == n)
                return candidate;
        }
        candidate++;
    }

    /* Unreachable, but keep compiler happy. */
    return (seq_t)0;
}
