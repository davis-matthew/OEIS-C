/*
 * A043450  Numbers having two 7's in base 8.
 * https://oeis.org/A043450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers having two 7's in base 8 (OEIS A043450)
 * Range:     u8 n <= 4, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   has_two_sevens_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: brute‑force enumeration in increasing order. */

#include <stdbool.h>
#include "../utils/has_two_sevens_idx.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined offset – return 0 */

    idx_t found = 0;                    /* number of terms found so far */
    for (idx_t v = 0; ; ++v) {          /* scan all integers in increasing order */
        if (has_two_sevens_idx(v)) {
            ++found;
            if (found == n) return (seq_t)v;
        }
    }
}
