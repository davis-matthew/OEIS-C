/*
 * A133906  Least number m such that binomial(n+m, m) mod m = 1.
 * https://oeis.org/A133906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 254, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, mod_pow, test_m
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = least m ≥ 2 such that binomial(n+m, m) ≡ 1 (mod m) */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mod_pow.h"
#include "../utils/test_m.h"

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
