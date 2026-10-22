/*
 * A004785  5!(2n-6)!/n!(n-1)! is an integer.
 * https://oeis.org/A004785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Finds n-th positive integer k such that 5!(2k-6)!/[k!(k-1)!] is
 *            an integer
 * Range:     u8 n <= 5, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>

#define MAX_PRIME 100000

static bool prime_sieve[MAX_PRIME + 1];
static bool sieve_initialized = false;

static void sieve_of_eratosthenes() {
    memset(prime_sieve, 1, sizeof(prime_sieve));
    prime_sieve[0] = prime_sieve[1] = 0;
    
    for (idx_t i = 2; i * i <= MAX_PRIME; i++) {
        if (prime_sieve[i]) {
            for (idx_t j = i * i; j <= MAX_PRIME; j += i) {
                prime_sieve[j] = 0;
            }
        }
    }
    
    sieve_initialized = true;
}

static idx_t legendre(idx_t n, idx_t p) {
    idx_t result = 0;
    idx_t pk = p;
    while (pk <= n) {
        result += n / pk;
        if (pk > n / p) break;
        pk *= p;
    }
    return result;
}

static bool check_condition(idx_t k) {
    if (k < 3) return false;
    
    idx_t m = 2 * k - 6;
    idx_t max_prime = (m > 5) ? m : 5;
    if (max_prime > MAX_PRIME) max_prime = MAX_PRIME;
    
    for (idx_t p = 2; p <= max_prime; p++) {
        if (!prime_sieve[p]) continue;
        
        idx_t v_num = legendre(5, p) + legendre(m, p);
        idx_t v_den = legendre(k, p) + legendre(k - 1, p);
        
        if (v_num < v_den) return false;
    }
    
    return true;
}

seq_t a(idx_t n) {
    if (!sieve_initialized) {
        sieve_of_eratosthenes();
    }
    
    idx_t count = 0;
    for (idx_t k = 1; ; k++) {
        if (check_condition(k)) {
            count++;
            if (count == n) return (seq_t)k;
        }
    }
}
