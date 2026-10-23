/*
 * A039250  Numbers whose base-12 representation has the same number of 4's
 *          and 8's.
 * https://oeis.org/A039250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth nonnegative integer whose base‑12 representation
 *            contains an equal number of digits 4 and 8. Terms are indexed
 *            from 1 (a(1) = 0)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   equal_base12
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/equal_base12.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    for (idx_t cand = 0;; ++cand) {
        if (equal_base12((seq_t)cand)) {
            ++found;
            if (found == n)
                return (seq_t)cand;
        }
    }
}
