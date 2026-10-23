/*
 * A121038  Multiples of 18 containing a 18 in their decimal representation.
 * https://oeis.org/A121038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 201, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_18_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A121038: the nth multiple of 18 whose decimal representation
   contains the substring "18".  The function searches incrementally for
   the next valid multiple until it reaches the requested index. */
#include <stdbool.h>
#include "../utils/has_18_digits.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* undefined for n<1, return 0 as placeholder */

    seq_t val = 18;             /* first multiple of 18 */
    idx_t idx = 0;              /* count of found terms */

    while (1) {
        if (has_18_digits(val)) {
            ++idx;
            if (idx == n)
                return val;
        }
        val += 18;              /* next multiple of 18 */
    }
}
