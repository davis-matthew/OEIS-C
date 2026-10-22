/*
 * A007445  Inverse Moebius transform of primes.
 * https://oeis.org/A007445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} prime(d)
 * Range:     u8 n <= 35, u16 n <= 2519, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   sieve_and_extract_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "../utils/sieve_and_extract_primes.h"

seq_t a(idx_t n) {
    static seq_t *primes = NULL;
    static idx_t prime_count = 0;
    
    if (primes == NULL) {
        primes = sieve_and_extract_primes(1000000, &prime_count);
    }
    
    seq_t result = 0;
    
    for (idx_t d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            if (d <= prime_count) {
                result += primes[d - 1];
            }
            
            idx_t other_d = n / d;
            if (d != other_d && other_d <= prime_count) {
                result += primes[other_d - 1];
            }
        }
    }
    
    return result;
}
