/*
 * A007770  Happy numbers: numbers whose trajectory under iteration of sum of
 *          squares of digits map (see A003132) includes 1.
 * https://oeis.org/A007770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 9821, u32 n <= 9855, u64 n <= 9855,
 *            u128 n <= 9855, big any n
 * Helpers:   is_happy, sum_of_squares_of_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_happy.h"
#include "../utils/sum_of_squares_of_digits.h"

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
