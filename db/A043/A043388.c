/*
 * A043388  Numbers having four 4's in base 6.
 * https://oeis.org/A043388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   count4_base6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count4_base6.h"
/* Sequence A043388: nth integer whose base‑6 representation contains
   exactly four 4’s.  Computed by brute‑force enumeration. */

seq_t a(idx_t n) {
    if (n == 0)
        return 0;          /* offset is 1; below offset behaviour unchecked */
    seq_t x = 0;
    idx_t found = 0;
    while (found < n) {
        x++;
        if (count4_base6(x) == 4)
            found++;
    }
    return x;
}
