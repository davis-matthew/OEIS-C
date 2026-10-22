/*
 * A002037  Product of all primes up to 3^n.
 * https://oeis.org/A002037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of all primes p <= 3^n
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    // Compute 3^n
    uint64_t limit = 1;
    for (idx_t i = 0; i < n; i++) {
        if (limit > UINT64_MAX / 3) {
            limit = UINT64_MAX;
            break;
        }
        limit *= 3;
    }
    
    // Sieve of Eratosthenes to find all primes up to limit
    bool *is_prime = (bool *)malloc(limit + 1);
    if (!is_prime) return 0;
    
    memset(is_prime, true, limit + 1);
    is_prime[0] = is_prime[1] = false;
    
    for (uint64_t i = 2; i <= limit / i; i++) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) {
                is_prime[j] = false;
            }
        }
    }
    
    // Multiply all primes together
    seq_t result = 1;
    for (uint64_t i = 2; i <= limit; i++) {
        if (is_prime[i]) {
            result *= (seq_t)i;
        }
    }
    
    free(is_prime);
    return result;
}
