/*
 * A039471  Numbers whose base-11 representation has the same nonzero number
 *          of 3's and 10's.
 * https://oeis.org/A039471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   equal_3_and_10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑11 representation has the same nonzero number of 3's and 10's. */
#include <stdbool.h>
#include "../utils/equal_3_and_10.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset is 1; no term for n=0 */
    seq_t cand;
    idx_t found = 0;
    for (cand = 1; ; cand++) {
        if (equal_3_and_10(cand)) {
            ++found;
            if (found == n)
                return cand;
        }
    }
}
