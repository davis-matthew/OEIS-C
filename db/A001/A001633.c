/*
 * A001633  Numbers with an odd number of digits.
 * https://oeis.org/A001633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   The n-th number with an odd number of digits
 * Range:     u8 n <= 166, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_for_digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_for_digits.h"
#include "../utils/pow10.h"

seq_t a(idx_t n) {
    idx_t pos = n;
    int digits = 1;
    idx_t group_size = 10;
    
    // Find which digit-group contains the n-th term
    while (pos > group_size) {
        pos -= group_size;
        digits += 2;
        group_size = count_for_digits(digits);
    }
    
    // Compute the first number with 'digits' digits
    seq_t start = (digits == 1) ? 0 : pow10(digits - 1);
    
    // Return the pos-th number in this group (1-indexed within group)
    return start + (seq_t)(pos - 1);
}
