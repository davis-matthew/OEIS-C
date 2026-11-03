/*
 * A004753  Numbers whose binary expansion contains 100.
 * https://oeis.org/A004753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Search for n-th number whose binary expansion contains 100
 * Range:     u8 n <= 168, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 81 terms through n = 885 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool contains_100(seq_t x) {
    while (x >= 4) {
        if ((x & 7) == 4) {
            return true;
        }
        x >>= 1;
    }
    return false;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t candidate = 0;
    
    while (count < (seq_t)n) {
        candidate++;
        if (contains_100(candidate)) {
            count++;
        }
    }
    
    return candidate;
}
