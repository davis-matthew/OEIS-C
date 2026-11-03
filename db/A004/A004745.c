/*
 * A004745  Numbers whose binary expansion does not contain 001.
 * https://oeis.org/A004745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th non-negative integer whose binary representation
 *            does not contain the substring "001"
 * Range:     u8 n <= 134, u16 n <= 6747, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9541 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static int msb_position(seq_t x) {
    if (x == 0) return -1;
    int pos = 0;
    while (x > 0) {
        x >>= 1;
        pos++;
    }
    return pos - 1;
}

static bool has_001(seq_t num) {
    int msb = msb_position(num);
    if (msb < 2) return false;
    
    for (int shift = 0; shift <= msb - 2; shift++) {
        if (((num >> shift) & 7) == 1) {
            return true;
        }
    }
    
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t num = 0;
    
    while (1) {
        if (!has_001(num)) {
            count++;
            if (count == n) {
                return num;
            }
        }
        num++;
    }
}
