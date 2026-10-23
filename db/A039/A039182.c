/*
 * A039182  Numbers whose base-11 representation has the same number of 3's
 *          and 4's.
 * https://oeis.org/A039182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   equal34
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the nth nonnegative integer whose base‑11
   representation contains an equal number of digits 3 and 4. */

#include <stdbool.h>
#include "../utils/equal34.h"

seq_t a(idx_t n)
{
    /* The sequence is 1‑indexed; n >= 1 by specification. */
    seq_t val = 0;          /* Candidate number.       */
    idx_t  found = 0;       /* How many good numbers seen. */

    while (1) {
        if (equal34(val)) {
            found++;
            if (found == n) {
                return val;
            }
        }
        val++;              /* Move to next integer.  */
    }
}
