/*
 * A125293  Numbers with at least one 1 and one 2 in ternary representation.
 * https://oeis.org/A125293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   has_both_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers with at least one 1 and one 2 in ternary representation
   a(n) = the nth positive integer whose ternary form contains at
   least one digit '1' and at least one digit '2'.
   The function scans integers until it finds n such numbers. */

#include <stdbool.h>
#include "../utils/has_both_digits.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t cand = 1;
    for (;; ++cand) {
        if (has_both_digits(cand)) {
            ++count;
            if (count == n)
                return cand;
        }
    }
}
