/*
 * A048311  Numbers whose base-14 expansions, read from left to right, have
 *          run lengths that strictly increase.
 * https://oeis.org/A048311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_valid_base14
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumeration: numbers with strictly increasing run-lengths in base-14 */
#include <stdbool.h>
#include "../utils/is_valid_base14.h"

seq_t a(idx_t n)
{
    seq_t num = 0;
    idx_t count = 0;
    while (1) {
        if (is_valid_base14(num)) {
            ++count;
            if (count == n + 1) {
                return num;
            }
        }
        ++num;
    }
}
