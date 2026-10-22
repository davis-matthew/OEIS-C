/*
 * A004749  Numbers whose binary expansion contains the substring '110'.
 * https://oeis.org/A004749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   search for numbers whose binary representation contains "110"
 * Range:     u8 n <= 168, u16 n <= 9459, u32 n <= 9459, u64 n <= 9459,
 *            u128 n <= 9459, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


static bool contains_110(seq_t num) {
    while (num >= 6) {
        if ((num & 7) == 6) {
            return true;
        }
        num >>= 1;
    }
    return false;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t m = 0;
    
    while (count < n) {
        m++;
        if (contains_110(m)) {
            count++;
        }
    }
    
    return m;
}
