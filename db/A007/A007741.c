/*
 * A007741  a(n) = prime(n)*...*prime(m), the least product of consecutive
 *          primes which is abundant.
 * https://oeis.org/A007741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=n..A007707(n)} prime(k), the least product of
 *            consecutive primes starting at prime(n) that is abundant
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"


seq_t a(idx_t n) {
    // Find the n-th prime
    uint64_t candidate = 2;
    idx_t prime_count = 0;
    
    while (true) {
        if (is_prime(candidate)) {
            prime_count++;
            if (prime_count == n) {
                break;
            }
        }
        candidate++;
    }
    
    uint64_t start_prime = candidate;
    
    // Keep multiplying consecutive primes until the product is abundant
    seq_t product = 1;
    seq_t sigma = 1;  // Sum of divisors
    
    candidate = start_prime;
    
    while (true) {
        // candidate is a prime; multiply it into the product
        product *= (seq_t)candidate;
        sigma *= (1 + (seq_t)candidate);
        
        // An abundant number satisfies sigma(n) > 2*n
        if (sigma > 2 * product) {
            return product;
        }
        
        // Find the next prime
        do {
            candidate++;
        } while (!is_prime(candidate));
    }
}
