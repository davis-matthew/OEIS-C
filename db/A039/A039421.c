/*
 * A039421  Numbers whose base-10 representation has the same nonzero number
 *          of 3's and 8's.
 * https://oeis.org/A039421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6425, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   equal_nonzero_38
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers whose base‑10 representation has the same non‑zero
   number of 3's and 8's.  */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/equal_nonzero_38.h"

/*  OEIS A039421: Numbers whose base‑10 representation has the
    same nonzero number of 3's and 8's.  */
seq_t a(idx_t n)
{
    if (n == 0U) {
        /* Offset is 1; undefined behavior for n = 0, return 0. */
        return (seq_t)0;
    }

    idx_t found = 0U;
    uint64_t cur   = 1U;            /* start from the first positive integer */

    while (true) {
        if (equal_nonzero_38(cur)) {
            ++found;
            if (found == n) {
                return (seq_t)cur;
            }
        }
        ++cur;
    }
}
