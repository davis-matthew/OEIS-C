/*
 * A007645  Generalized cuban primes: primes of the form x^2 + xy + y^2; or
 *          primes of the form x^2 + 3*y^2; or primes == 0 or 1 (mod 3).
 * https://oeis.org/A007645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes p such that p ≡ 0 or 1 (mod 3)
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 952 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t candidate = 2;
    
    while (1) {
        if (is_prime_u64((uint64_t)candidate)) {
            seq_t mod3 = candidate % 3;
            if (mod3 == 0 || mod3 == 1) {
                count++;
                if (count == n) {
                    return candidate;
                }
            }
        }
        candidate++;
    }
}
