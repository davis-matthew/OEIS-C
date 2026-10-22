/*
 * A007641  Primes of the form 2*k^2 + 29.
 * https://oeis.org/A007641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   n-th prime of the form 2*k^2 + 29
 * Range:     u8 n <= 11, u16 n <= 122, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t k = 0;
    
    while (1) {
        seq_t value = 2 * ((seq_t)k * (seq_t)k) + 29;
        
        if (is_prime((uint64_t)value)) {
            count++;
            if (count == n) {
                return value;
            }
        }
        k++;
    }
}
