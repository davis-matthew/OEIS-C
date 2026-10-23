/*
 * A044054  Numbers k such that string 0,2 occurs in the base-3 representation
 *          of k but not of k-1.
 * https://oeis.org/A044054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 5528, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_substr_02
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n‑th integer k such that base‑3 representation of k
   contains the substring "02" while k-1 does not. The sequence is
   defined by this condition. */

#include <stdbool.h>
#include "../utils/has_substr_02.h"

/* Main function required by the build system. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t k = 1;

    while (found < n) {
        if (has_substr_02(k) && !has_substr_02(k - 1)) {
            ++found;
            if (found == n)
                return k;
        }
        ++k;
    }

    /* The loop is guaranteed to terminate for all n >= 1. */
    return 0;
}
