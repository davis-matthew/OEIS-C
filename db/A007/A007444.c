/*
 * A007444  Moebius transform of primes.
 * https://oeis.org/A007444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} prime(d)*mu(n/d)
 * Range:     u8 n <= 58, u16 n <= 6546, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


static uint64_t prime_cache[20001];
static uint64_t prime_cache_len = 0;

static bool is_prime_simple(uint64_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (uint64_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

static void build_prime_cache(uint64_t needed) {
    if (prime_cache_len >= needed) return;
    
    if (prime_cache_len == 0) {
        prime_cache[0] = 2;
        prime_cache_len = 1;
    }
    
    uint64_t candidate = prime_cache[prime_cache_len - 1] + 1;
    if (candidate % 2 == 0) candidate++;
    
    while (prime_cache_len < needed) {
        if (is_prime_simple(candidate)) {
            prime_cache[prime_cache_len] = candidate;
            prime_cache_len++;
        }
        candidate += 2;
    }
}

static uint64_t get_prime(uint64_t n) {
    build_prime_cache(n);
    return prime_cache[n - 1];
}

static int moebius(uint64_t n) {
    if (n == 1) return 1;
    
    int prime_count = 0;
    uint64_t temp = n;
    
    for (uint64_t p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            prime_count++;
            temp /= p;
            if (temp % p == 0) return 0;
        }
    }
    if (temp > 1) prime_count++;
    
    return (prime_count % 2 == 0) ? 1 : -1;
}

seq_t a(idx_t n) {
    seq_t pos = 0, neg = 0;
    
    for (uint64_t d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            seq_t prime_d = (seq_t)get_prime(d);
            int mu = moebius(n / d);
            if (mu > 0) pos += prime_d;
            else if (mu < 0) neg += prime_d;
            
            if (d != n / d) {
                uint64_t d2 = n / d;
                seq_t prime_d2 = (seq_t)get_prime(d2);
                int mu2 = moebius(n / d2);
                if (mu2 > 0) pos += prime_d2;
                else if (mu2 < 0) neg += prime_d2;
            }
        }
    }
    
    return pos - neg;
}
