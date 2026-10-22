/*
 * A001259  A sequence of sorted odd primes 3 = p_1 < p_2 < ... < p_m such
 *          that p_i-2 divides the product p_1*p_2*...*p_(i-1) of the earlier
 *          primes and each prime factor of p_i-1 is a prime factor of twice
 *          the product.
 * https://oeis.org/A001259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   Sorted odd primes p_i where (p_i - 2) divides the product of
 *            earlier primes, and each prime factor of (p_i - 1) is a prime
 *            factor of twice that product
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   check_divisibility, sieve_of_eratosthenes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/check_divisibility.h"
#include "../utils/sieve_of_eratosthenes.h"

static int get_prime_factors(uint64_t n, uint64_t *factors) {
    int count = 0;
    for (uint64_t p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            factors[count++] = p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) factors[count++] = n;
    return count;
}

static bool check_prime_factors(uint64_t p_minus_1, const uint64_t *primes, idx_t num_primes) {
    uint64_t factors[64];
    int nfactors = get_prime_factors(p_minus_1, factors);
    
    for (int i = 0; i < nfactors; i++) {
        uint64_t pf = factors[i];
        uint64_t prod_mod = 2 % pf;
        for (idx_t j = 0; j < num_primes; j++) {
            prod_mod = (prod_mod * (primes[j] % pf)) % pf;
        }
        if (prod_mod != 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
        
    if (n == 1) return (seq_t)3;
    
    const uint64_t MAX_SIEVE = 1000000;
    bool *sieve = (bool *)malloc(MAX_SIEVE + 1);
    if (!sieve) return 0;
    
    sieve_of_eratosthenes(sieve, MAX_SIEVE);
    
    uint64_t *primes = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)n);
    if (!primes) {
        free(sieve);
        return 0;
    }
    
    primes[0] = 3;
    idx_t count = 1;
    
    for (uint64_t candidate = 5; candidate <= MAX_SIEVE && count < n; candidate += 2) {
        if (sieve[candidate]) {
            if (check_divisibility(candidate - 2, primes, count) &&
                check_prime_factors(candidate - 1, primes, count)) {
                primes[count++] = candidate;
            }
        }
    }
    
    seq_t result = (count >= n) ? (seq_t)primes[n - 1] : 0;
    free(sieve);
    free(primes);
    return result;
}
