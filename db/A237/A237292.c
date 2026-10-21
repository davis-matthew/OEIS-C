/*
 * A237292  a(n) = A002326(2n(n+1)) / A002326(n).
 * https://oeis.org/A237292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ord_{(2n+1)^2}(2) / ord_{2n+1}(2)
 * Range:     u8 n <= 7, u16 n <= 3755, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_uint64_simple, gcd_uint64, lcm_uint64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mod_pow.h"   /* mod_pow(seq_t base, uint64_t exp, seq_t mod) */
#include "../utils/factor_uint64_simple.h"
#include "../utils/gcd_uint64.h"
#include "../utils/lcm_uint64.h"

/* GCD and LCM for uint64_t */

/* Multiplicative order of 2 modulo prime^exp (prime odd). */
static uint64_t order_of_two_mod_prime_power(uint64_t prime, uint64_t exp) {
    /* q = prime^exp */
    uint64_t q = 1;
    for (uint64_t i = 0; i < exp; ++i) q *= prime;
    /* φ(q) = prime^(exp-1) * (prime-1) */
    uint64_t phi = 1;
    for (uint64_t i = 1; i < exp; ++i) phi *= prime;
    phi *= prime - 1ULL;
    /* factor φ(q) */
    uint64_t pf[32], pe[32], pcnt;
    factor_uint64_simple(phi, pf, pe, &pcnt);
    uint64_t order = phi;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t f = pf[i];
        while (order % f == 0) {
            uint64_t cand = order / f;
            if (mod_pow((seq_t)2, cand, (seq_t)q) == (seq_t)1) {
                order = cand;
            } else {
                break;
            }
        }
    }
    return order;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */
    uint64_t p = 2ULL * n + 1ULL;          /* 2n+1 */
    /* factor p */
    uint64_t pf[32], pe[32], pcnt;
    factor_uint64_simple(p, pf, pe, &pcnt);

    uint64_t ord_p  = 1;                   /* order modulo p */
    uint64_t ord_p2 = 1;                   /* order modulo p^2 */
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t prime = pf[i];
        uint64_t e     = pe[i];

        uint64_t ord_single  = order_of_two_mod_prime_power(prime, e);
        ord_p = lcm_uint64(ord_p, ord_single);

        uint64_t ord_single2 = order_of_two_mod_prime_power(prime, 2 * e);
        ord_p2 = lcm_uint64(ord_p2, ord_single2);
    }

    uint64_t result = ord_p2 / ord_p;
    return (seq_t)result;
}
