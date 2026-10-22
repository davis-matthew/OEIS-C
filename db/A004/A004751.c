/*
 * A004751  Binary expansion contains 001.
 * https://oeis.org/A004751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 122, u16 n <= 9730, u32 n <= 9730, u64 n <= 9730,
 *            u128 n <= 9730, big any n
 * Helpers:   has_001
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_001.h"

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
