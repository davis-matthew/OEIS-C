/*
 * A003940  Order of simple Chevalley group B_4(q), q = prime power.
 * https://oeis.org/A003940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = q^16*(q^2-1)*(q^4-1)*(q^6-1)*(q^8-1)/gcd(2,q-1), where q
 *            = A000961(n+1)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 2, u128 n <= 6, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static bool is_prime_power_u64(uint64_t n) {
    uint64_t p;
    if (n < 2) return false;
    for (p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            return n == 1;
        }
    }
    return true; /* n itself is prime */
}

static uint64_t *pp_cache = NULL;
static idx_t pp_len = 0;   /* number of cached terms */
static idx_t pp_cap = 0;

/* A000961(k), k >= 1: 1, 2, 3, 4, 5, 7, 8, 9, ... */
static uint64_t prime_power_at(idx_t k) {
    uint64_t cand;
    if (pp_cache == NULL) {
        pp_cap = 64;
        pp_cache = (uint64_t *)malloc(pp_cap * sizeof(uint64_t));
        pp_cache[0] = 1;
        pp_len = 1;
    }
    cand = pp_cache[pp_len - 1];
    while (pp_len < k) {
        cand++;
        if (is_prime_power_u64(cand)) {
            if (pp_len >= pp_cap) {
                pp_cap *= 2;
                pp_cache = (uint64_t *)realloc(pp_cache, pp_cap * sizeof(uint64_t));
            }
            pp_cache[pp_len++] = cand;
        }
    }
    return pp_cache[k - 1];
}

seq_t a(idx_t n) {
    uint64_t qu = prime_power_at(n + 1);
    seq_t q = (seq_t)qu;
    seq_t q2 = q * q;
    seq_t q4 = q2 * q2;
    seq_t q6 = q4 * q2;
    seq_t q8 = q4 * q4;
    seq_t q16 = q8 * q8;
    seq_t f1 = q2 - 1;
    seq_t r;
    if (qu % 2 == 1) f1 /= 2;
    r = q16;
    r *= f1;
    r *= (q4 - 1);
    r *= (q6 - 1);
    r *= (q8 - 1);
    return r;
}
