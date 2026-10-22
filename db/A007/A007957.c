/*
 * A007957  Numbers that contain an odd digit.
 * https://oeis.org/A007957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 216, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   contains_odd_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: n-th positive integer containing at least one odd digit */

#include <stdbool.h>
#include "../utils/contains_odd_digit.h"

seq_t a(idx_t n) {
    seq_t num = 0;
    idx_t count = 0;
    while (count < n) {
        num++;
        if (contains_odd_digit(num)) {
            count++;
        }
    }
    return num;
}
