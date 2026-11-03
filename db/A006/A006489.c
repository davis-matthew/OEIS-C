/*
 * A006489  Numbers k such that k-6, k, and k+6 are primes.
 * https://oeis.org/A006489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) is the n-th number k where k-6, k, and k+6 are all prime
 * Range:     u8 n <= 14, u16 n <= 393, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 937 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n) {
    seq_t k = 7;
    idx_t count = 0;
    
    while (1) {
        if (is_prime_u64((uint64_t)(k - 6)) && 
            is_prime_u64((uint64_t)k) && 
            is_prime_u64((uint64_t)(k + 6))) {
            count++;
            if (count == n) {
                return k;
            }
        }
        k++;
    }
}
