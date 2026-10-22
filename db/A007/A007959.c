/*
 * A007959  Some nontrivial permutation of digits is an odd number.
 * https://oeis.org/A007959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Search: n-th number where some nontrivial permutation of digits yields an odd number. */

static bool is_in_sequence(seq_t num) {
    if (num < 10) {
        return false;
    }
    
    // Check if num is odd and has more than 2 digits (i.e., >= 100)
    if ((num % 2 == 1) && (num >= 100)) {
        return true;
    }
    
    // Check if num has an odd digit not in the units place
    seq_t temp = num / 10;  // Remove the units digit
    while (temp > 0) {
        if ((temp % 10) % 2 == 1) {
            return true;
        }
        temp /= 10;
    }
    
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t num = 9;
    
    while (count < n) {
        num++;
        if (is_in_sequence(num)) {
            count++;
        }
    }
    
    return num;
}
