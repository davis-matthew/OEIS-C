/*
 * A004751  Binary expansion contains 001.
 * https://oeis.org/A004751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 122, u16 n <= 32769, u32 n <= 32769, u64 n <= 32769,
 *            u128 n <= 32769, big any n
 * Validated: standard, pass, 75 terms through n = 9730 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has_001(seq_t num) {
    while (num >= 8) {
        if ((num & 7) == 1) {
            return true;
        }
        num >>= 1;
    }
    return false;
}

/* Search for integers whose binary representation contains "001" substring */

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t num = 0;
    
    while (count < (seq_t)n) {
        num++;
        if (has_001(num)) {
            count++;
        }
    }
    
    return num;
}
