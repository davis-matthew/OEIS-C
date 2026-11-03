/*
 * A133906  Least number m such that binomial(n+m, m) mod m = 1.
 * https://oeis.org/A133906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 254, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 114 terms through n = 978 (2026-11-01)
 * Helpers:   build_primes_upto, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = least m ≥ 2 such that binomial(n+m, m) ≡ 1 (mod m) */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/build_primes_upto.h"
#include "../utils/powmod.h"

static bool test_m(idx_t n, idx_t m, const uint64_t *primes, size_t primes_cnt) {
    seq_t mod = (seq_t)m;
    seq_t result = 1;

    /* Compute binomial(n+m, m) mod m using prime exponents */
    idx_t limit = n + m;
    for (size_t pi = 0; pi < primes_cnt && primes[pi] <= limit; ++pi) {
        uint64_t p = primes[pi];
        uint64_t e = 0;
        idx_t t = p;
        while (t <= limit) {
            e += limit / t - n / t - m / t;
            if (t > limit / p) break;   /* prevent overflow of t *= p */
            t *= p;
        }
        if (e == 0) continue;

        seq_t pow = 1;
        seq_t base = (seq_t)(p % m);
        uint64_t exp = e;
        while (exp) {
            if (exp & 1)
                pow = (pow * base) % mod;
            base = (base * base) % mod;
            exp >>= 1;
        }
        result = (result * pow) % mod;
    }

    return result == 1;
}

seq_t a(idx_t n) {
    /* Pre‑compute primes up to 20000 once */
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0;
    if (!prime_list) {
        build_primes_upto(20000, &prime_list, &prime_cnt);
    }

    for (idx_t m = 2; ; ++m) {
        if (test_m(n, m, prime_list, prime_cnt))
            return (seq_t)m;
    }
}
