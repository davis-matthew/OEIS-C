/*
 * A007928  Numbers containing an even digit.
 * https://oeis.org/A007928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 201, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* a(n) = n-th number containing at least one even digit */

static bool has_even_digit(seq_t num) {
    if (num == 0) return true;
    while (num > 0) {
        seq_t digit = num % 10;
        if (digit % 2 == 0) return true;
        num /= 10;
    }
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t current = 0;
    
    while (1) {
        if (has_even_digit(current)) {
            count++;
            if (count == n) {
                return current;
            }
        }
        current++;
    }
}
