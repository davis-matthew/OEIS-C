/*
 * A002145  Primes of the form 4*k + 3.
 * https://oeis.org/A002145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 3284, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sieve of Eratosthenes to find primes of the form 4k+3, return the n-th one */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    uint64_t limit = (n < 100) ? 500 : (n < 1000) ? 30000 : (n < 10000) ? 500000 : n * 100;
    
    while (limit <= 1000000000) {
        bool *is_prime = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
        if (!is_prime) return 0;
        
        for (uint64_t i = 0; i <= limit; i++) {
            is_prime[i] = true;
        }
        is_prime[0] = is_prime[1] = false;
        
        for (uint64_t i = 2; i * i <= limit; i++) {
            if (is_prime[i]) {
                for (uint64_t j = i * i; j <= limit; j += i) {
                    is_prime[j] = false;
                }
            }
        }
        
        idx_t count = 0;
        for (uint64_t i = 3; i <= limit; i += 2) {
            if (is_prime[i] && i % 4 == 3) {
                count++;
                if (count == n) {
                    seq_t result = (seq_t)i;
                    free(is_prime);
                    return result;
                }
            }
        }
        
        free(is_prime);
        limit *= 2;
    }
    
    return 0;
}
