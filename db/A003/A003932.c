/*
 * A003932  Order of universal Chevalley group B_3(q), q = prime power.
 * https://oeis.org/A003932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = q^9 * (q^2 - 1) * (q^4 - 1) * (q^6 - 1) where q =
 *            A000961(n+1)
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 6, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>


static bool is_prime_power(uint64_t n, uint64_t *base) {
    if (n == 1) {
        *base = 2;
        return true;
    }
    
    for (uint64_t p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            uint64_t m = n;
            while (m % p == 0) {
                m /= p;
            }
            if (m == 1) {
                *base = p;
                return true;
            }
            return false;
        }
    }
    
    *base = n;
    return true;
}

static uint64_t *prime_power_cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_max = 0;

static void extend_prime_power_cache(idx_t k) {
    if (cache_max == 0) {
        cache_max = 100;
        prime_power_cache = (uint64_t *)malloc(sizeof(uint64_t) * cache_max);
    }
    
    while (cache_size < k) {
        if (cache_size >= cache_max) {
            cache_max *= 2;
            prime_power_cache = (uint64_t *)realloc(prime_power_cache, sizeof(uint64_t) * cache_max);
        }
        
        uint64_t n = (cache_size == 0) ? 1 : prime_power_cache[cache_size - 1] + 1;
        uint64_t base;
        while (!is_prime_power(n, &base)) {
            n++;
        }
        
        prime_power_cache[cache_size] = n;
        cache_size++;
    }
}

static uint64_t prime_power_k(idx_t k) {
    extend_prime_power_cache(k);
    return prime_power_cache[k - 1];
}

seq_t a(idx_t n) {
    uint64_t q = prime_power_k(n + 1);
    
    seq_t q_seq = (seq_t)q;
    seq_t q2 = q_seq * q_seq;
    seq_t q4 = q2 * q2;
    seq_t q6 = q4 * q2;
    seq_t q9 = q6 * q_seq * q_seq * q_seq;
    
    seq_t result = q9 * (q2 - (seq_t)1) * (q4 - (seq_t)1) * (q6 - (seq_t)1);
    
    return result;
}
