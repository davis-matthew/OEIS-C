/*
 * A001135  Primes p such that the multiplicative order of 2 modulo p is
 *          (p-1)/5.
 * https://oeis.org/A001135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Find the n-th prime p such that ord_p(2) = (p-1)/5
 * Range:     u8 n <= 1, u16 n <= 126, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


static seq_t cache[10100];
static idx_t cache_len = 0;

static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    if (n < 9) return true;
    if (n % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

static uint64_t powmod(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = ((__uint128_t)result * base) % mod;
        base = ((__uint128_t)base * base) % mod;
        exp >>= 1;
    }
    return result;
}

static bool check_order(uint64_t p) {
    if (p <= 2 || (p - 1) % 5 != 0) return false;
    
    uint64_t target = (p - 1) / 5;
    if (powmod(2, target, p) != 1) return false;
    
    uint64_t temp = target;
    for (uint64_t q = 2; q * q <= temp; q++) {
        if (temp % q == 0) {
            if (powmod(2, target / q, p) == 1) return false;
            while (temp % q == 0) temp /= q;
        }
    }
    if (temp > 1 && powmod(2, target / temp, p) == 1) return false;
    
    return true;
}

seq_t a(idx_t n) {
    if (n <= cache_len) return cache[n - 1];
    
    uint64_t p = (cache_len > 0) ? (uint64_t)cache[cache_len - 1] + 2 : 3;
    
    while (cache_len < n) {
        if (is_prime(p) && check_order(p)) {
            cache[cache_len] = (seq_t)p;
            cache_len++;
        }
        p += 2;
    }
    
    return cache[n - 1];
}
