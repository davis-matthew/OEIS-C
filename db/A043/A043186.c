/*
 * A043186  Numbers k such that 0 and 7 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_adjacent_07_or_70
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate over integers k, returning those where the base‑9 representation
 * contains the digits 0 and 7 adjacent (either "07" or "70") whereas k‑1 does not.
 * The sequence begins 63, 88, 144, … as in OEIS A043186.
 */

#include <stdbool.h>
#include "../utils/has_adjacent_07_or_70.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* offset is 1; 0 is never a term */

    idx_t found = 0;
    seq_t k = 1;                /* start scanning from 1 */

    while (found < n) {
        if (has_adjacent_07_or_70(k) &&
            !has_adjacent_07_or_70(k - (seq_t)1)) {
            ++found;
            if (found == n)
                return k;
        }
        ++k;
    }
    /* unreachable: the loop always returns */
    return (seq_t)0;
}
