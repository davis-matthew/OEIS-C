/*
 * A039058  Numbers whose base-8 representation has the same number of 1's and
 *          4's.
 * https://oeis.org/A039058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   equal_ones_fours
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search: a(n) is the nth non‑negative integer whose
   base‑8 representation contains an equal number of 1's and 4's. */

#include <stdbool.h>
#include "../utils/equal_ones_fours.h"

/* Sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n < 1 */

    seq_t val = 0;
    idx_t count = 0;
    for (;;) {
        if (equal_ones_fours(val)) {
            ++count;
            if (count == n)
                return val;
        }
        ++val;
    }
}
