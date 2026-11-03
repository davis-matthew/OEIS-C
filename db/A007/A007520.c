/*
 * A007520  Primes == 3 (mod 8).
 * https://oeis.org/A007520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1635, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9736 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n) {
    /* Computation: find n-th prime p where p ≡ 3 (mod 8) */
    idx_t count = 0;
    seq_t candidate = 3;
    
    while (true) {
        if (is_prime_u64((uint64_t)candidate)) {
            count++;
            if (count == n) {
                return candidate;
            }
        }
        candidate += 8;
    }
}
