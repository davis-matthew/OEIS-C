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
 * Helpers:   has_three_consecutive_zeros
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_three_consecutive_zeros.h"

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
