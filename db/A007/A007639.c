/*
 * A007639  Primes of form 2n^2 - 2n + 19.
 * https://oeis.org/A007639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 100, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 951 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_u64.h"

/* Search for primes of the form 2n^2 - 2n + 19 */
seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t k = 1;
    
    while (1) {
        seq_t candidate = 2 * (seq_t)k * (seq_t)k - 2 * (seq_t)k + 19;
        
        if (is_prime_u64((uint64_t)candidate)) {
            count++;
            if (count == n) {
                return candidate;
            }
        }
        k++;
    }
}
