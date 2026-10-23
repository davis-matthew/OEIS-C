/*
 * A043444  Numbers having four 5's in base 8.
 * https://oeis.org/A043444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   has_four_fives
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers having exactly four 5's in base-8 (octal) representation */

#include <stdbool.h>
#include "../utils/has_four_fives.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t num = 0; ; ++num) {
        if (has_four_fives(num)) {
            ++count;
            if (count == n)
                return (seq_t)num;
        }
    }
}
