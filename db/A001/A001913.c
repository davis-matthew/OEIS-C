/*
 * A001913  Full reptend primes: primes with primitive root 10.
 * https://oeis.org/A001913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   Primes p where 10 is a primitive root mod p, i.e., order(10, p)
 *            = p-1
 * Range:     u8 n <= 20, u16 n <= 2475, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

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
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

static bool is_primitive_root_10(uint64_t p) {
    if (p == 2 || p == 5) return false;
    
    uint64_t phi = p - 1;
    uint64_t temp = phi;
    
    for (uint64_t q = 2; q * q <= temp; q++) {
        if (temp % q == 0) {
            if (powmod(10, phi / q, p) == 1) return false;
            while (temp % q == 0) temp /= q;
        }
    }
    
    if (temp > 1) {
        if (powmod(10, phi / temp, p) == 1) return false;
    }
    
    return true;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t p = 3;
    
    while (count < n) {
        if (is_prime(p) && is_primitive_root_10(p)) {
            count++;
            if (count == n) return (seq_t)p;
        }
        p += 2;
    }
    
    return (seq_t)0;
}
