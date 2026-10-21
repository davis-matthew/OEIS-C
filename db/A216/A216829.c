/*
 * A216829  2*a(n) is the multiplicative order of 2 mod 3*(2n-1).
 * https://oeis.org/A216829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ord_{3*(2n-1)}(2) / 2 (ord is the multiplicative order of
 *            2)
 * Range:     u8 n <= 259, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd_uint64.h"

/* modular exponentiation: base^exp mod mod (mod <= 60000) */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t res = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1U)
            res = (res * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return res;
}

/* greatest common divisor for uint64_t */

/* factorize n into primes[]^exps[], count in cnt (n <= 60000) */
static void factor_uint64(uint64_t n, uint64_t primes[32], int exps[32], size_t *cnt) {
    *cnt = 0;
    if (n < 2)
        return;
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) {
            int e = 0;
            while (n % d == 0) {
                n /= d;
                ++e;
            }
            primes[(*cnt)] = d;
            exps[(*cnt)] = e;
            ++(*cnt);
        }
    }
    if (n > 1) {
        primes[(*cnt)] = n;
        exps[(*cnt)] = 1;
        ++(*cnt);
    }
}

/* multiplicative order of 2 modulo prime p (p odd, >2) */
static uint64_t order_mod_prime(uint64_t p) {
    uint64_t phi = p - 1;
    uint64_t order = phi;
    uint64_t temp = phi;
    for (uint64_t q = 2; q * q <= temp; ++q) {
        if (temp % q == 0) {
            while (order % q == 0 && mod_pow_uint64(2, order / q, p) == 1)
                order /= q;
            while (temp % q == 0)
                temp /= q;
        }
    }
    if (temp > 1) { /* remaining prime factor */
        uint64_t q = temp;
        while (order % q == 0 && mod_pow_uint64(2, order / q, p) == 1)
            order /= q;
    }
    return order;
}

/* multiplicative order of 2 modulo p^e (p odd) */
static uint64_t order_mod_prime_power(uint64_t p, int e) {
    uint64_t order = order_mod_prime(p);
    if (e == 1)
        return order;
    uint64_t mod = p * p;          /* p^2 */
    for (int exp = 2; exp <= e; ++exp) {
        if (mod_pow_uint64(2, order, mod) != 1)
            order *= p;
        if (exp < e)               /* prepare next modulus */
            mod *= p;
    }
    return order;
}

/* main sequence function */
seq_t a(idx_t n) {
    /* M = 3 * (2n - 1) */
    uint64_t M = 3 * ((uint64_t)n * 2U - 1U);

    /* factor M */
    uint64_t primes[32];
    int exps[32];
    size_t cnt;
    factor_uint64(M, primes, exps, &cnt);

    /* compute order of 2 modulo M by lcm of orders for each prime power */
    uint64_t order = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        int e = exps[i];
        uint64_t op = order_mod_prime_power(p, e);
        uint64_t g = gcd_uint64(order, op);
        order = order / g * op;
    }

    /* a(n) = order / 2 */
    return (seq_t)(order / 2U);
}
