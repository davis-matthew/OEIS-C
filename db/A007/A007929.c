/*
 * A007929  Odd numbers containing an even digit.
 * https://oeis.org/A007929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has_even_digit(seq_t num) {
    while (num > 0) {
        seq_t digit = num % 10;
        if ((digit & 1) == 0) {
            return true;
        }
        num /= 10;
    }
    return false;
}

/* Search: the n-th odd number that contains at least one even digit (0, 2, 4, 6, or 8) */

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t num = 19;
    
    while (count < n) {
        num += 2;
        if (has_even_digit(num)) {
            count++;
        }
    }
    
    return num;
}
