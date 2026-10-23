/*
 * A045097  Numbers whose base-4 representation contains exactly two 1's and
 *          two 2's.
 * https://oeis.org/A045097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer whose base‑4 representation contains
 *            exactly two 1's and two 2's
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   has_two_one_two_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_two_one_two_two.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, but return 0 safely */

    idx_t found = 0;
    uint64_t val = 0;

    for (val = 1;; ++val) {
        if (has_two_one_two_two(val)) {
            ++found;
            if (found == n)
                return (seq_t)val;
        }
    }
}
