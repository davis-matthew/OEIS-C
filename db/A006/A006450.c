/*
 * A006450  Prime-indexed primes: primes with prime subscripts.
 * https://oeis.org/A006450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = prime(prime(n))
 * Range:     u8 n <= 16, u16 n <= 844, u32 n <= 9553, u64 n <= 9553,
 *            u128 n <= 9553, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    static bool* sieve = NULL;
    static idx_t sieve_limit = 0;
    static idx_t* primes = NULL;
    static idx_t num_primes = 0;
    
    if (sieve == NULL) {
        sieve_limit = 2000000;
        sieve = (bool*)malloc((sieve_limit + 1) * sizeof(bool));
        memset(sieve, true, (sieve_limit + 1) * sizeof(bool));
        sieve[0] = sieve[1] = false;
        
        for (idx_t i = 2; i * i <= sieve_limit; i++) {
            if (sieve[i]) {
                for (idx_t j = i * i; j <= sieve_limit; j += i) {
                    sieve[j] = false;
                }
            }
        }
        
        num_primes = 0;
        for (idx_t i = 2; i <= sieve_limit; i++) {
            if (sieve[i]) num_primes++;
        }
        
        primes = (idx_t*)malloc(num_primes * sizeof(idx_t));
        idx_t idx = 0;
        for (idx_t i = 2; i <= sieve_limit; i++) {
            if (sieve[i]) {
                primes[idx++] = i;
            }
        }
        
        free(sieve);
        sieve = NULL;
    }
    
    if (n > num_primes) {
        return 0;
    }
    
    idx_t p_n = primes[n - 1];
    
    if (p_n > num_primes) {
        return 0;
    }
    
    return (seq_t)primes[p_n - 1];
}
