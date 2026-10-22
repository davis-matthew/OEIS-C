/*
 * A000419  Numbers that are the sum of 3 but no fewer nonzero squares.
 * https://oeis.org/A000419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   numbers that require exactly 3 nonzero squares
 * Range:     u8 n <= 117, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_perfect_square, is_sum_of_two_squares, seq_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_perfect_square.h"
#include "../utils/is_sum_of_two_squares.h"
#include "../utils/seq_isqrt.h"

static bool is_sum_of_three_squares(seq_t n) {
    seq_t temp = n;
    while (temp % 4 == 0) {
        temp /= 4;
    }
    return (temp % 8) != 7;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t candidate = 0;
    
    while (count < (seq_t)n) {
        candidate++;
        if (is_sum_of_three_squares(candidate) &&
            !is_perfect_square(candidate) &&
            !is_sum_of_two_squares(candidate)) {
            count++;
        }
    }
    
    return candidate;
}
