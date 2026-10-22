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
 * Helpers:   msb_position
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/msb_position.h"

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
