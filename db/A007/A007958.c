/*
 * A007958  Even numbers with at least one odd digit.
 * https://oeis.org/A007958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Iterate through even numbers and count those with at least one odd digit */

#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t candidate = 10;
    
    while (1) {
        seq_t temp = candidate;
        bool has_odd_digit = false;
        
        while (temp > 0) {
            seq_t digit = temp % 10;
            if (digit % 2 == 1) {
                has_odd_digit = true;
                break;
            }
            temp /= 10;
        }
        
        if (has_odd_digit) {
            count++;
            if (count == n) {
                return candidate;
            }
        }
        
        candidate += 2;
    }
}
