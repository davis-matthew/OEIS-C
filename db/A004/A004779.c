/*
 * A004779  Binary expansion contains 3 adjacent 0's.
 * https://oeis.org/A004779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   n-th number whose binary expansion contains three consecutive 0
 *            bits
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9972 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has_three_consecutive_zeros(seq_t x) {
    if (x == 0) return false;
    
    int highest_bit = 0;
    seq_t temp = x;
    while (temp > 0) {
        temp >>= 1;
        highest_bit++;
    }
    
    for (int i = 0; i <= highest_bit - 3; i++) {
        if (((x >> i) & 7) == 0) {
            return true;
        }
    }
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t value = 8;
    
    while (true) {
        if (has_three_consecutive_zeros(value)) {
            if (count == n) {
                return value;
            }
            count++;
        }
        value++;
    }
}
