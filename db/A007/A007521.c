/*
 * A007521  Primes of the form 8k + 5.
 * https://oeis.org/A007521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1643, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9208 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form 8k + 5 */

#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t candidate = 5;
    
    while (1) {
        if (is_prime_u64((uint64_t)candidate)) {
            count++;
            if (count == n) {
                return candidate;
            }
        }
        candidate += 8;
    }
}
