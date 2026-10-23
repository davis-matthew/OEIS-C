/*
 * A109303  Numbers k with at least one duplicate base-10 digit (A107846(k) >
 *          0).
 * https://oeis.org/A109303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n-th positive integer that contains at least one
 *            repeated decimal digit
 * Range:     u8 n <= 57, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_duplicate_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_duplicate_digits.h"

seq_t a(idx_t n) {
    seq_t cur = 11;        /* first integer with a repeated digit */
    idx_t cnt = 0;
    while (1) {
        if (has_duplicate_digits(cur)) {
            ++cnt;
            if (cnt == n)
                return cur;
        }
        ++cur;
    }
}
