/*
 * A043382  Numbers having two 3's in base 6.
 * https://oeis.org/A043382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_two_three
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th smallest integer that has exactly two digits
   equal to 3 when written in base‑6. The function searches
   sequentially for the required term. */
#include <stdbool.h>
#include "../utils/has_two_three.h"

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t value = 0;
    for (;;) {
        if (has_two_three(value)) {
            ++found;
            if (found == n) {
                return value;
            }
        }
        ++value;
    }
}
