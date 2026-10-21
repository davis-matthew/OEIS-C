/*
 * A050749  Squares containing no pair of consecutive equal digits.
 * https://oeis.org/A050749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = square of the nth integer whose square contains no pair
 *            of consecutive equal digits
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   no_consecutive_equal_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/no_consecutive_equal_digits.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t x = 0;
    while (true) {
        seq_t sq = x * x;
        if (no_consecutive_equal_digits(sq)) {
            ++count;
            if (count == n) {
                return sq;
            }
        }
        ++x;
    }
}
