/*
 * A007770  Happy numbers: numbers whose trajectory under iteration of sum of
 *          squares of digits map (see A003132) includes 1.
 * https://oeis.org/A007770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 9821, u32 n <= 143071, u64 n <= 143071,
 *            u128 n <= 143071, big any n
 * Validated: standard, pass, 70 terms through n = 9855 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t sum_of_squares_of_digits(seq_t num) {
    seq_t sum = 0;
    while (num > 0) {
        seq_t digit = num % 10;
        sum += digit * digit;
        num /= 10;
    }
    return sum;
}

static bool is_happy(seq_t num) {
    seq_t slow = num;
    seq_t fast = num;
    
    while (1) {
        slow = sum_of_squares_of_digits(slow);
        if (slow == 1) return true;
        
        fast = sum_of_squares_of_digits(fast);
        if (fast == 1) return true;
        
        fast = sum_of_squares_of_digits(fast);
        if (fast == 1) return true;
        
        if (slow == fast) return false;
    }
}

/* A007770: nth happy number. Search for positive integers whose trajectory
   under iteration of sum of squares of digits reaches 1. */

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t candidate = 0;
    
    while (count < n) {
        candidate++;
        if (is_happy(candidate)) {
            count++;
        }
    }
    
    return candidate;
}
