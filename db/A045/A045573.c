/*
 * A045573  Still numbers when turned upside down (only uses digits 0125689,
 *          with no final 0's).
 * https://oeis.org/A045573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   is_allowed_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045573: numbers in decimal consisting only of the digits
   0, 1, 2, 5, 6, 8, 9, and having no trailing zero.
   They are returned in increasing order; a(1) = 1. */
#include <stdbool.h>
#include "../utils/is_allowed_digit.h"

static inline bool valid_seq(seq_t x) {
    /* Reject numbers ending in zero. */
    if ((x % 10) == 0) return false;
    seq_t tmp = x;
    while (tmp != 0) {
        int digit = (int)(tmp % 10);
        if (!is_allowed_digit(digit)) return false;
        tmp /= 10;
    }
    return true;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t value = 1;
    while (true) {
        if (valid_seq(value)) {
            ++count;
            if (count == (seq_t)n) {
                return value;
            }
        }
        ++value;
    }
}
