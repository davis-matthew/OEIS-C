/*
 * A001746  At least one digit contains a loop (version 1).
 * https://oeis.org/A001746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = n-th non-negative integer containing at least one looped
 *            digit (0, 6, 8, 9)
 * Range:     u8 n <= 149, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_loop_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/has_loop_digit.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t m = 0;
    
    while (1) {
        if (has_loop_digit(m)) {
            count++;
            if (count == (seq_t)n) {
                return m;
            }
        }
        m++;
    }
}
