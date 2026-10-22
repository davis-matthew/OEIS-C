/*
 * A000961  Powers of primes. Alternatively, 1 and the prime powers (p^k, p
 *          prime, k >= 1).
 * https://oeis.org/A000961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, core, nice
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 6635, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sieve-based approach: mark all prime powers up to a limit, then return the n-th one */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    idx_t limit = 200000;
    
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    memset(is_prime, true, (limit + 1) * sizeof(bool));
    is_prime[0] = is_prime[1] = false;
    
    for (idx_t i = 2; i * i <= limit; i++) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                is_prime[j] = false;
            }
        }
    }
    
    bool *is_prime_power = (bool *)malloc((limit + 1) * sizeof(bool));
    memset(is_prime_power, false, (limit + 1) * sizeof(bool));
    is_prime_power[1] = true;
    
    for (idx_t p = 2; p <= limit; p++) {
        if (is_prime[p]) {
            idx_t power = p;
            while (power <= limit) {
                is_prime_power[power] = true;
                if (power > limit / p) break;
                power *= p;
            }
        }
    }
    
    idx_t count = 0;
    for (idx_t i = 1; i <= limit; i++) {
        if (is_prime_power[i]) {
            count++;
            if (count == n) {
                free(is_prime);
                free(is_prime_power);
                return (seq_t)i;
            }
        }
    }
    
    free(is_prime);
    free(is_prime_power);
    return 0;
}
