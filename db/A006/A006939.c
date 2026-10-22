/*
 * A006939  Chernoff sequence: a(n) = Product_{k=1..n} prime(k)^(n-k+1).
 * https://oeis.org/A006939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = Product of first n primorials = Product_{i=1..n}
 *            Product_{j=1..i} prime(j)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (uint64_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    
    static uint64_t* primes = NULL;
    static idx_t cached_count = 0;
    
    if (cached_count < n) {
        if (primes != NULL) free(primes);
        primes = (uint64_t*)malloc(n * sizeof(uint64_t));
        
        idx_t found = 0;
        uint64_t candidate = 2;
        while (found < n) {
            if (is_prime(candidate)) {
                primes[found++] = candidate;
            }
            candidate = (candidate == 2) ? 3 : candidate + 2;
        }
        cached_count = n;
    }
    
    seq_t result = (seq_t)1;
    seq_t primorial = (seq_t)1;
    
    for (idx_t i = 0; i < n; i++) {
        primorial = primorial * (seq_t)primes[i];
        result = result * primorial;
    }
    
    return result;
}
