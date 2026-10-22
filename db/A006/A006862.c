/*
 * A006862  Euclid numbers: 1 + product of the first n primes.
 * https://oeis.org/A006862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 1 + product of the first n primes
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"


seq_t a(idx_t n) {
    seq_t product = 1;
    idx_t prime_count = 0;
    uint64_t candidate = 2;
    
    while (prime_count < n) {
        if (is_prime(candidate)) {
            product *= (seq_t)candidate;
            prime_count++;
        }
        candidate++;
    }
    
    return product + 1;
}
