/*
 * A003726  Numbers with no 3 adjacent 1's in binary expansion.
 * https://oeis.org/A003726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th number with no three consecutive 1's in binary;
 *            computed by iteration
 * Range:     u8 n <= 149, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9529 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has_three_ones(seq_t num) {
    while (num > 0) {
        if ((num & 7) == 7) {
            return true;
        }
        num >>= 1;
    }
    return false;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t num = 0;
    
    while (1) {
        if (!has_three_ones(num)) {
            count++;
            if (count == n) {
                return num;
            }
        }
        num++;
    }
}
