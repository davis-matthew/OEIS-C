/*
 * A004752  Binary expansion contains 010.
 * https://oeis.org/A004752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 9255, u32 n <= 9255, u64 n <= 9255,
 *            u128 n <= 9255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: nth number whose binary representation contains the substring "010" */
#include <stdbool.h>

static bool contains_010(seq_t num) {
    if (num < 2) return false;
    
    seq_t shifted = num;
    while (shifted > 2) {
        if ((shifted & 7) == 2) {
            return true;
        }
        shifted >>= 1;
    }
    return false;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t num = 0;
    seq_t target = (seq_t)n;
    
    while (count < target) {
        num++;
        if (contains_010(num)) {
            count++;
        }
    }
    
    return num;
}
