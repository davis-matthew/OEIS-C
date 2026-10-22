/*
 * A003625  Primes congruent to {3, 5, 6} mod 7.
 * https://oeis.org/A003625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n-th prime p where p ≡ 3, 5, or 6 (mod 7) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t candidate = 2;
    
    while (1) {
        if (is_prime(candidate)) {
            uint64_t rem = candidate % 7;
            if (rem == 3 || rem == 5 || rem == 6) {
                count++;
                if (count == n) {
                    return (seq_t)candidate;
                }
            }
        }
        
        if (candidate == 2) {
            candidate = 3;
        } else {
            candidate += 2;
        }
    }
}
