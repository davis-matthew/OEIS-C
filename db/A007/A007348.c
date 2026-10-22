/*
 * A007348  Primes for which -10 is a primitive root.
 * https://oeis.org/A007348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search for primes p where -10 is a primitive root modulo p
 * Range:     u8 n <= 23, u16 n <= 2475, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

static uint64_t modpow(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

static bool is_primitive_root(uint64_t p, uint64_t a) {
    if (a == 0) return false;
    
    uint64_t phi = p - 1;
    uint64_t temp = phi;
    
    for (uint64_t q = 2; q * q <= temp; q++) {
        if (temp % q == 0) {
            uint64_t exp = phi / q;
            if (modpow(a, exp, p) == 1) return false;
            while (temp % q == 0) temp /= q;
        }
    }
    
    if (temp > 1) {
        uint64_t exp = phi / temp;
        if (modpow(a, exp, p) == 1) return false;
    }
    
    return true;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t p = 2;
    
    while (1) {
        if (is_prime(p)) {
            uint64_t ten_mod_p = 10 % p;
            uint64_t mod_neg10 = ten_mod_p ? (p - ten_mod_p) : 0;
            
            if (mod_neg10 != 0 && is_primitive_root(p, mod_neg10)) {
                count++;
                if (count == n) return (seq_t)p;
            }
        }
        p++;
    }
}
