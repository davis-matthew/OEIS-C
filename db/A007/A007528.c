/*
 * A007528  Primes of the form 6k-1.
 * https://oeis.org/A007528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 925 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: the n-th prime of the form 6k-1 */
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t k = 1;
    
    while (1) {
        seq_t candidate = 6 * (seq_t)k - 1;
        if (is_prime_u64((uint64_t)candidate)) {
            count++;
            if (count == n) {
                return candidate;
            }
        }
        k++;
    }
}
