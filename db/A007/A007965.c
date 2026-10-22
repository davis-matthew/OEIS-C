/*
 * A007965  a(n) = n^prime(n) - prime(n)^n.
 * https://oeis.org/A007965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n^prime(n) - prime(n)^n
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 5, i64 n <= 8, i128 n <= 11,
 *            big any n
 * Helpers:   a000248_pow, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"
#include "../utils/a000248_pow.h"

seq_t a(idx_t n) {
        
    // Find the n-th prime
    idx_t count = 0;
    uint64_t candidate = 2;
    uint64_t prime_n = 0;
    
    while (count < n) {
        if (is_prime(candidate)) {
            count++;
            if (count == n) {
                prime_n = candidate;
                break;
            }
        }
        candidate++;
    }
    
    // Compute n^prime(n) - prime(n)^n
    seq_t term1 = a000248_pow((seq_t)n, prime_n);
    seq_t term2 = a000248_pow((seq_t)prime_n, n);
    
    return term1 - term2;
}
