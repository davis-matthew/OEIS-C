/*
 * A043122  Numbers k such that 1 and 3 occur juxtaposed in the base-6
 *          representation of k but not of k-1.
 * https://oeis.org/A043122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   has_adj13_or31
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A043122: numbers k with the digits 1 and 3 appearing adjacent in base‑6,
   and k–1 not containing such an adjacency.  This is computed by a direct
   brute‑force scan of integers. */
#include <stdbool.h>
#include "../utils/has_adj13_or31.h"

seq_t a(idx_t n)
{
    seq_t count = 0;  /* how many qualifying k's we have found */
    seq_t k = 1;      /* current integer to test */

    while (1) {
        if (has_adj13_or31(k) && !has_adj13_or31(k - 1)) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}
