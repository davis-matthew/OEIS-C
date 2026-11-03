/*
 * A237292  a(n) = A002326(2n(n+1)) / A002326(n).
 * https://oeis.org/A237292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ord_{(2n+1)^2}(2) / ord_{2n+1}(2)
 * Range:     u8 n <= 7, u16 n <= 3755, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9631 (2026-11-01)
 * Helpers:   gcd_u64, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"
#include "../utils/powmod.h"

/* Simple trial‑division factorisation for numbers up to ~4e8. */
static void factor_uint64_simple(uint64_t n, uint64_t *pf, uint64_t *pe, size_t *cnt) {
    *cnt = 0;
    if (n == 0) return;
    /* factor 2 */
    if ((n & 1ULL) == 0) {
        uint64_t e = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            e++;
        }
        pf[(*cnt)] = 2ULL;
        pe[(*cnt)] = e;
        (*cnt)++;
    }
    /* odd factors */
    for (uint64_t p = 3ULL; p * p <= n; p += 2ULL) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                e++;
            }
            pf[(*cnt)] = p;
            pe[(*cnt)] = e;
            (*cnt)++;
        }
    }
    if (n > 1) {
        pf[(*cnt)] = n;
        pe[(*cnt)] = 1;
        (*cnt)++;
    }
}

static uint64_t lcm_uint64(uint64_t a, uint64_t b) {
    if (a == 0 || b == 0) return 0;
    return a / gcd_u64(a, b) * b;
}

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
            if (powmod((seq_t)2, cand, (seq_t)q) == (seq_t)1) {
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
