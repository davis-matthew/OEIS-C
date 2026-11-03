/*
 * A000419  Numbers that are the sum of 3 but no fewer nonzero squares.
 * https://oeis.org/A000419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   numbers that require exactly 3 nonzero squares
 * Range:     u8 n <= 117, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9923 (2026-11-01)
 * Helpers:   is_square, isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_square.h"
#include "../utils/isqrt_seq.h"

static bool is_sum_of_two_squares(seq_t n) {
    seq_t temp = n;
    
    while (temp % 2 == 0) {
        temp /= 2;
    }
    
    for (seq_t p = 3; p * p <= temp; p += 2) {
        int count = 0;
        while (temp % p == 0) {
            count++;
            temp /= p;
        }
        if ((count & 1) && (p % 4 == 3)) {
            return false;
        }
    }
    
    if (temp > 1 && (temp % 4 == 3)) {
        return false;
    }
    
    return true;
}

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
            !is_square(candidate) &&
            !is_sum_of_two_squares(candidate)) {
            count++;
        }
    }
    
    return candidate;
}
