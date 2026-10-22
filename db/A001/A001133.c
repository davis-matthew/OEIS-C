/*
 * A001133  Primes p such that the multiplicative order of 2 modulo p is
 *          (p-1)/3.
 * https://oeis.org/A001133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes p where ord_p(2) = (p-1)/3
 * Range:     u8 n <= 4, u16 n <= 447, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


static uint64_t powmod(uint64_t b, uint64_t e, uint64_t m) {
    uint64_t r = 1;
    b %= m;
    while (e > 0) {
        if (e & 1) r = ((__uint128_t)r * b) % m;
        b = ((__uint128_t)b * b) % m;
        e >>= 1;
    }
    return r;
}

static bool is_prime(uint64_t p) {
    if (p < 2) return false;
    if (p == 2) return true;
    if (p % 2 == 0) return false;
    if (p < 9) return true;
    if (p % 3 == 0) return false;
    
    for (uint64_t i = 5; i * i <= p; i += 6) {
        if (p % i == 0 || p % (i + 2) == 0) return false;
    }
    return true;
}

static int get_prime_factors(uint64_t n, uint64_t *factors) {
    int count = 0;
    for (uint64_t i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            factors[count++] = i;
            while (n % i == 0) n /= i;
        }
    }
    if (n > 1) factors[count++] = n;
    return count;
}

static bool check_order(uint64_t p) {
    if ((p - 1) % 3 != 0) return false;
    
    uint64_t k = (p - 1) / 3;
    
    if (powmod(2, k, p) != 1) return false;
    
    uint64_t factors[64];
    int nf = get_prime_factors(k, factors);
    
    for (int i = 0; i < nf; i++) {
        if (powmod(2, k / factors[i], p) == 1) {
            return false;
        }
    }
    
    return true;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t p = 2;
    
    while (1) {
        if (is_prime(p) && check_order(p)) {
            count++;
            if (count == n) {
                return (seq_t)p;
            }
        }
        p = (p == 2) ? 3 : p + 2;
    }
}
