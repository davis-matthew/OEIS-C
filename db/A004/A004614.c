/*
 * A004614  Numbers that are divisible only by primes congruent to 3 mod 4.
 * https://oeis.org/A004614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th number whose prime factors are all congruent to
 *            3 mod 4
 * Range:     u8 n <= 61, u16 n <= 9973, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_valid_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_valid_seq.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t candidate = 0;
    
    while (count < (seq_t)n) {
        candidate++;
        if (is_valid_seq(candidate)) {
            count++;
        }
    }
    
    return candidate;
}
