/*
 * A044570  Numbers n such that string 0,2 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   has_02
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers n whose base‑8 representation contains the substring "02"
   while the representation of n+1 does not.  The function a(n) returns the
   n-th such number (with a(1) = 66).  */

#include <stdbool.h>
#include "../utils/has_02.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for the problem, return 0 for safety */

    idx_t found = 0;
    for (seq_t x = 1;; ++x) {
        if (has_02(x)) {
            seq_t x1 = x + 1;
            if (!has_02(x1)) {
                ++found;
                if (found == n)
                    return x;
            }
        }
    }
}
