/*
 * A107901  Length of continued fraction for prime(n)/n.
 * https://oeis.org/A107901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of terms in the continued fraction of prime(n)/n *
 *            (a(n) = A107898(n) - 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9608 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static seq_t cf_length(uint64_t a, uint64_t b) {
    uint64_t len = 0;
    while (b != 0) {
        ++len;
        uint64_t r = a % b;
        a = b;
        b = r;
    }
    return (seq_t)len;
}

static uint64_t *prime_list = NULL;
static size_t prime_cap   = 0;
static size_t prime_cnt   = 0;

static void ensure_primes(size_t n) {
    if (prime_cnt >= n) return;

    if (!prime_list) {
        prime_cap = 128;
        prime_list = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
    }

    uint64_t cand = (prime_cnt == 0) ? 2 : prime_list[prime_cnt - 1] + 1;
    while (prime_cnt < n) {
        if (cand == 2) {
            prime_list[prime_cnt++] = 2;
            cand = 3;
            continue;
        }
        if ((cand & 1) == 0) {
            cand++;
            continue;
        }
        if (is_prime_u64(cand)) {
            if (prime_cnt >= prime_cap) {
                prime_cap *= 2;
                prime_list = (uint64_t*)realloc(prime_list, prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_cnt++] = cand;
        }
        cand++;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    ensure_primes((size_t)n);
    uint64_t p = prime_list[n - 1];
    return cf_length(p, (uint64_t)n);
}
