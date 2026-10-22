/*
 * A004787  Numbers k such that 7!*(2k-8)!/(k!*(k-1)!) is an integer.
 * https://oeis.org/A004787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Find k such that 7!*(2k-8)!/(k!*(k-1)!) is an integer; a(n) = n-th such k */

#define MAX_PRIME 100000

static bool sieve_primes[MAX_PRIME + 1];
static bool sieve_initialized = false;

static void init_sieve(void) {
    if (sieve_initialized) return;
    
    memset(sieve_primes, true, sizeof(sieve_primes));
    sieve_primes[0] = sieve_primes[1] = false;
    
    for (uint64_t i = 2; i * i <= MAX_PRIME; i++) {
        if (sieve_primes[i]) {
            for (uint64_t j = i * i; j <= MAX_PRIME; j += i) {
                sieve_primes[j] = false;
            }
        }
    }
    
    sieve_initialized = true;
}

static bool is_prime(uint64_t p) {
    if (p <= MAX_PRIME) {
        init_sieve();
        return sieve_primes[p];
    }
    
    if (p < 2) return false;
    if (p == 2) return true;
    if (p % 2 == 0) return false;
    for (uint64_t i = 3; i * i <= p; i += 2) {
        if (p % i == 0) return false;
    }
    return true;
}

static uint64_t legendre(uint64_t n, uint64_t p) {
    uint64_t exp = 0;
    uint64_t pk = p;
    while (pk <= n) {
        exp += n / pk;
        if (pk > n / p) break;
        pk *= p;
    }
    return exp;
}

static bool is_in_sequence(uint64_t k) {
    if (k < 4) return false;
    
    uint64_t max_val = 2*k - 8;
    if (max_val < k) max_val = k;
    if (max_val < 7) max_val = 7;
    
    for (uint64_t p = 2; p <= max_val; p++) {
        if (!is_prime(p)) continue;
        
        uint64_t num_exp = legendre(7, p) + legendre(2*k - 8, p);
        uint64_t den_exp = legendre(k, p) + legendre(k - 1, p);
        
        if (num_exp < den_exp) return false;
    }
    
    return true;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t k = 4;
    
    while (count < n) {
        if (is_in_sequence(k)) {
            count++;
            if (count == n) return (seq_t)k;
        }
        k++;
    }
    
    return 0;
}
