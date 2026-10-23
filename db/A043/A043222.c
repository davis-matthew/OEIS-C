/*
 * A043222  Numbers k such that 0 and 7 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_adjacent_07_70
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043222.
   a(n) = the n-th positive integer k such that k has the digits 0 and 7 adjacent
   (i.e., contains the substring "07" or "70") while k‑1 does NOT have such an adjacency. */

#include <stdbool.h>
#include "../utils/has_adjacent_07_70.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for offset 1, but safe guard */

    idx_t found = 0;
    for (idx_t k = 1; ; ++k) {
        if (has_adjacent_07_70(k) && !has_adjacent_07_70(k - 1)) {
            ++found;
            if (found == n) return (seq_t)k;
        }
    }
}
