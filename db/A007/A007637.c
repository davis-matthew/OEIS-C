/*
 * A007637  Primes of form 3*k^2 - 3*k + 23.
 * https://oeis.org/A007637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 95, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th prime of form 3*k^2 - 3*k + 23 */

#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t k = 1;
    
    while (1) {
        uint64_t candidate = 3 * k * k - 3 * k + 23;
        
        if (is_prime(candidate)) {
            count++;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
        k++;
    }
}
