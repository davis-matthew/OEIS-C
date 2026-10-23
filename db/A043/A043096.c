/*
 * A043096  Numbers in which every pair of adjacent digits are distinct.
 * https://oeis.org/A043096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th non‑negative integer whose decimal digits
 *            contain no pair of equal adjacent digits (brute force
 *            enumeration)
 * Range:     u8 n <= 213, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   no_consecutive_equal_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/no_consecutive_equal_digits.h"

seq_t a(idx_t n)
{
    seq_t val = 0;          /* a(1) = 0 */
    idx_t count = 1;
    if (n == 1) return val;

    while (true) {
        val = val + 1;      /* next integer */
        if (no_consecutive_equal_digits(val)) {
            ++count;
            if (count == n) return val;
        }
    }
}
