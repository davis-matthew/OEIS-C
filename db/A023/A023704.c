/*
 * A023704  Numbers with exactly 6 2's in their ternary expansion.
 * https://oeis.org/A023704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3448, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_six_twos
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers with exactly 6 2's in their ternary expansion */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_six_twos.h"

/* a(n) is the n-th positive integer whose base‑3 expansion
   contains exactly six digits equal to 2.  The sequence is
   indexed starting at 1 (a(1) = 728).  This implementation
   simply scans the integers in increasing order until the
   required position is found. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for offset 1, return 0 as fallback */
        return 0;

    idx_t   found = 0;
    seq_t   v     = 0;

    for (;; ++v)
    {
        if (has_six_twos(v))
        {
            ++found;
            if (found == n)
                return v;
        }
    }
}
