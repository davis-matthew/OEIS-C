/*
 * A001123  Primes with 3 as smallest primitive root.
 * https://oeis.org/A001123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   primes p where 3 is the smallest primitive root
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime_factors.h"

static uint64_t modpow(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) {
            result = ((__uint128_t)result * base) % mod;
        }
        base = ((__uint128_t)base * base) % mod;
        exp >>= 1;
    }
    return result;
}

static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    if (n < 9) return true;
    if (n % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

static bool is_primitive_root(uint64_t g, uint64_t p) {
    uint64_t phi = p - 1;
    uint64_t factors[64];
    int nfactors = get_prime_factors(phi, factors);
    for (int i = 0; i < nfactors; i++) {
        uint64_t exp = phi / factors[i];
        if (modpow(g, exp, p) == 1) {
            return false;
        }
    }
    return true;
}

seq_t a(idx_t n) {
    uint64_t count = 0;
    uint64_t p = 7;
    
    while (1) {
        if (is_prime(p)) {
            if (is_primitive_root(3, p) && !is_primitive_root(2, p)) {
                count++;
                if (count == n) {
                    return (seq_t)p;
                }
            }
        }
        p += 2;
    }
}
