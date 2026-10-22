/*
 * A004619  Numbers divisible only by primes congruent to 1 mod 7.
 * https://oeis.org/A004619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_only_primes_1mod7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n-th positive integer whose prime factors are all congruent to 1 mod 7 */

#include <stdbool.h>
#include "../utils/has_only_primes_1mod7.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t candidate = 0;
    
    while (count < (seq_t)n) {
        candidate++;
        if (has_only_primes_1mod7(candidate)) {
            count++;
        }
    }
    
    return candidate;
}
