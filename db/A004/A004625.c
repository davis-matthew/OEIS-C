/*
 * A004625  Numbers divisible only by primes congruent to 1 mod 8.
 * https://oeis.org/A004625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number divisible only by primes congruent to 1 mod 8
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_only_valid_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_only_valid_prime_factors.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t candidate = 0;
    
    while (count < n) {
        candidate++;
        if (has_only_valid_prime_factors(candidate)) {
            count++;
        }
    }
    
    return candidate;
}
