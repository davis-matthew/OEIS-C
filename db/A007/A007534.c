/*
 * A007534  Positive even numbers that are not the sum of a pair of twin
 *          primes.
 * https://oeis.org/A007534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sieve to find twin primes, then count even numbers not expressible as their sums */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    uint64_t limit = 200000;
    
    while (limit <= 100000000ULL) {
        bool *is_prime = (bool *)malloc(limit + 1);
        memset(is_prime, true, limit + 1);
        is_prime[0] = is_prime[1] = false;
        
        for (uint64_t i = 2; i * i <= limit; i++) {
            if (is_prime[i]) {
                for (uint64_t j = i * i; j <= limit; j += i) {
                    is_prime[j] = false;
                }
            }
        }
        
        bool *is_twin = (bool *)malloc(limit + 1);
        memset(is_twin, false, limit + 1);
        
        for (uint64_t i = 2; i + 2 <= limit; i++) {
            if (is_prime[i] && is_prime[i + 2]) {
                is_twin[i] = true;
                is_twin[i + 2] = true;
            }
        }
        
        idx_t count = 0;
        uint64_t candidate = 2;
        seq_t result = 0;
        
        while (candidate <= limit) {
            bool is_sum = false;
            
            for (uint64_t p = 2; p * 2 <= candidate; p++) {
                if (is_twin[p] && is_twin[candidate - p]) {
                    is_sum = true;
                    break;
                }
            }
            
            if (!is_sum) {
                count++;
                if (count == n) {
                    result = (seq_t)candidate;
                    break;
                }
            }
            
            candidate += 2;
        }
        
        free(is_prime);
        free(is_twin);
        
        if (result > 0) {
            return result;
        }
        
        limit *= 2;
    }
    
    return 0;
}
