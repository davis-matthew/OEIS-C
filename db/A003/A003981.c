/*
 * A003981  Inverse Möbius transform of A003965.
 * https://oeis.org/A003981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   Multiplicative with a(p^e) = (q^(e+1)-1)/(q-1) where q =
 *            Fibonacci(pi(p)+2)
 * Range:     u8 n <= 36, u16 n <= 82, u32 n <= 198, u64 n <= 478,
 *            u128 n <= 1102, big any n
 * Helpers:   check_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/check_prime.h"

static uint64_t prime_count_cache[64];
static uint64_t prime_cache_keys[64];
static int prime_cache_size = 0;

static uint64_t prime_count(uint64_t p) {
    for (int i = 0; i < prime_cache_size; i++) {
        if (prime_cache_keys[i] == p) return prime_count_cache[i];
    }
    
    uint64_t count = 0;
    for (uint64_t i = 2; i <= p; i++) {
        if (check_prime(i)) count++;
    }
    
    if (prime_cache_size < 64) {
        prime_cache_keys[prime_cache_size] = p;
        prime_count_cache[prime_cache_size] = count;
        prime_cache_size++;
    }
    
    return count;
}

static seq_t fibonacci(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    seq_t a = 0, b = 1;
    for (idx_t i = 2; i <= n; i++) {
        seq_t c = a + b;
        a = b;
        b = c;
    }
    return b;
}

static seq_t geometric_sum(seq_t q, idx_t e) {
    seq_t result = 1;
    seq_t power = q;
    for (idx_t i = 1; i <= e; i++) {
        result += power;
        if (i < e) power *= q;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t result = 1;
    uint64_t temp = (uint64_t)n;
    
    for (uint64_t p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                e++;
                temp /= p;
            }
            
            uint64_t pi_p = prime_count(p);
            seq_t q = fibonacci(pi_p + 2);
            seq_t term = geometric_sum(q, e);
            result *= term;
        }
    }
    
    if (temp > 1) {
        uint64_t pi_p = prime_count(temp);
        seq_t q = fibonacci(pi_p + 2);
        result *= (q + 1);
    }
    
    return result;
}
