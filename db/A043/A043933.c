/*
 * A043933  Numbers k such that 0 and 2 occur juxtaposed in the base-8
 *          representation of k but not of k+1.
 * https://oeis.org/A043933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   has_adjacent_zero_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) is the nth integer k whose base‑8 representation
   contains the digits 0 and 2 adjacent in any order, while k+1 does not.
   The function performs a simple linear search. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_adjacent_zero_two.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t k = 1;
    while (found < n) {
        if (has_adjacent_zero_two(k)) {
            seq_t kp1 = k + 1;
            if (!has_adjacent_zero_two(kp1)) {
                ++found;
                if (found == n)
                    return k;
            }
        }
        ++k;
    }
    return 0; /* unreachable for valid n */
}
