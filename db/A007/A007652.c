/*
 * A007652  Final digit of prime(n).
 * https://oeis.org/A007652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = prime(n) mod 10
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
        
    static uint8_t *is_prime_sieve = NULL;
    static uint64_t *primes = NULL;
    static idx_t prime_count = 0;
    static const uint64_t limit = 200000;
    
    if (is_prime_sieve == NULL) {
        is_prime_sieve = (uint8_t *)malloc(limit);
        memset(is_prime_sieve, 1, limit);
        is_prime_sieve[0] = is_prime_sieve[1] = 0;
        
        for (uint64_t i = 2; i * i < limit; i++) {
            if (is_prime_sieve[i]) {
                for (uint64_t j = i * i; j < limit; j += i) {
                    is_prime_sieve[j] = 0;
                }
            }
        }
        
        primes = (uint64_t *)malloc(limit * sizeof(uint64_t));
        for (uint64_t i = 2; i < limit; i++) {
            if (is_prime_sieve[i]) {
                primes[prime_count++] = i;
            }
        }
    }
    
    if (n <= prime_count) {
        return (seq_t)(primes[n - 1] % 10);
    }
    
    return 0;
}
