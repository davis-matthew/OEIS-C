/*
 * A044105  Numbers k such that string 0,3 occurs in the base 6 representation
 *          of k but not of k-1.
 * https://oeis.org/A044105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   has_03
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements sequence A044105: numbers k such that the base‑6 representation
   of k contains the substring "03" but the representation of k-1 does not. */
#include <stdbool.h>
#include "../utils/has_03.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* behavior below the offset is irrelevant */

    seq_t k = 0;
    idx_t count = 0;
    while (true) {
        ++k;                            /* test successive candidates */
        if (has_03(k) && !has_03(k - 1)) {
            ++count;
            if (count == n)
                return k;
        }
    }
}
