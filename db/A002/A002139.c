/*
 * A002139  Shuffling 2n cards.
 * https://oeis.org/A002139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (f(n)+1)*phi(p^(k-1)) if 2n-1 is a prime power p^k and 2
 *            is primitive mod 2n-1; otherwise lcm(f(n), f(n)+1) where f(n) =
 *            multiplicative order of 2 modulo 2n-1
 * Range:     u8 n <= 20, u16 n <= 260, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime_power.h"

static uint64_t mult_order(uint64_t a, uint64_t m) {
    if (m == 1) return 0;
    uint64_t order = 1;
    uint64_t power = a % m;
    while (power != 1) {
        power = (power * a) % m;
        order++;
    }
    return order;
}

static uint64_t phi(uint64_t m) {
    uint64_t result = m;
    uint64_t n = m;
    for (uint64_t p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            while (n % p == 0) {
                n /= p;
            }
            result = (result / p) * (p - 1);
        }
    }
    if (n > 1) {
        result = (result / n) * (n - 1);
    }
    return result;
}

static uint64_t gcd_u64(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n) {
    uint64_t m_u64 = 2 * n - 1;
    
    if (m_u64 == 1) {
        return 2;
    }
    
    uint64_t f = mult_order(2, m_u64);
    uint64_t phi_m = phi(m_u64);
    
    if (f == phi_m) {
        uint64_t p, k;
        if (is_prime_power(m_u64, &p, &k)) {
            seq_t phi_pk_minus_1;
            if (k == 1) {
                phi_pk_minus_1 = 1;
            } else {
                phi_pk_minus_1 = 1;
                for (uint64_t i = 0; i < k - 1; i++) {
                    phi_pk_minus_1 *= (seq_t)p;
                }
                phi_pk_minus_1 = (phi_pk_minus_1 / (seq_t)p) * ((seq_t)p - 1);
            }
            return ((seq_t)f + 1) * phi_pk_minus_1;
        }
    }
    
    uint64_t g = gcd_u64(f, f + 1);
    uint64_t f_over_g = f / g;
    return ((seq_t)f_over_g) * ((seq_t)f + 1);
}
