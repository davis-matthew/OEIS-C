/*
 * A039604  Numbers whose base-12 representation has the same nonzero number
 *          of 1's and 2's.
 * https://oeis.org/A039604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   has_ones_twos
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_ones_twos.h"

/* OEIS A039604: Numbers whose base‑12 representation has the same non‑zero
   number of 1’s and 2’s.  Computed by brute‑force enumeration. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset is 1; a(0) is undefined */

    idx_t count = 0;
    idx_t cand   = 1;                  /* start searching from 1 */

    while (count < n) {
        seq_t val = (seq_t)cand;
        if (has_ones_twos(val)) {
            count++;
            if (count == n)
                return val;
        }
        cand++;
    }
    return 0; /* unreachable */
}
