/*
 * A043941  Numbers k such that 1 and 4 occur juxtaposed in the base-8
 *          representation of k but not of k+1.
 * https://oeis.org/A043941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_adjacent_14_or_41
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search for k where base‑8 representation contains "14" or
   "41" but k+1 does not. */

#include <stdbool.h>
#include "../utils/has_adjacent_14_or_41.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1; undefined for n=0 */
    idx_t count = 0;
    seq_t k = 0;
    while (true) {
        if (has_adjacent_14_or_41(k) &&
            !has_adjacent_14_or_41(k + 1))
        {
            count++;
            if (count == n) return k;
        }
        k++;
    }
}
