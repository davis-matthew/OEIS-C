/*
 * A005574  Numbers k such that k^2 + 1 is prime.
 * https://oeis.org/A005574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   compute the n-th positive integer k such that k^2 + 1 is prime
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 4790, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9436 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t k = 0;
    
    while (count < n) {
        k++;
        seq_t k_squared_plus_1 = k * k + 1;
        if (is_prime_u64((uint64_t)k_squared_plus_1)) {
            count++;
        }
    }
    
    return k;
}
