/*
 * A005730  Related to n-th powers of polynomials: factors complementary to
 *          A005731.
 * https://oeis.org/A005730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n prime; 2*s(n) if 6|n; s(n) else; s(n) = product of
 *            distinct prime factors
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    seq_t kernel = 1;
    uint64_t m = n;
    
    // Check if n is prime
    if (is_prime(n)) {
        return 1;
    }
    
    // Compute squarefree kernel (product of distinct prime factors)
    for (uint64_t p = 2; p * p <= n; p++) {
        if (m % p == 0) {
            kernel *= (seq_t)p;
            while (m % p == 0) {
                m /= p;
            }
        }
    }
    
    // If m > 1, it's a prime factor
    if (m > 1) {
        kernel *= (seq_t)m;
    }
    
    // Apply the rule
    if (n % 6 == 0) {
        return 2 * kernel;
    } else {
        return kernel;
    }
}
