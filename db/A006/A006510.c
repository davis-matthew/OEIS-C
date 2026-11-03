/*
 * A006510  Duplicate of A006055.
 * https://oeis.org/A006510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   Primes formed by concatenating k consecutive digits (mod 10),
 *            ordered by length then starting position
 * Range:     i8 n <= 7, i16 n <= 8, i32 n <= 12, i64 n <= 13, i128 n <= 13,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_seq.h"

static seq_t construct_consecutive_digits(idx_t length, idx_t start_digit) {
    seq_t result = 0;
    for (idx_t i = 0; i < length; i++) {
        result = result * 10 + (seq_t)((start_digit + i) % 10);
    }
    return result;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    
    for (idx_t length = 1; length <= 1000; length++) {
        for (idx_t start = 0; start < 10; start++) {
            seq_t num = construct_consecutive_digits(length, start);
            if (is_prime_seq(num)) {
                count++;
                if (count == n)
                    return num;
            }
        }
    }
    
    return 0;
}
