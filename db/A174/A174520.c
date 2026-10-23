/*
 * A174520  Sum of all composite numbers in-between prime numbers p(n) and
 *          p(n+2).
 * https://oeis.org/A174520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of all composite numbers between prime p(n) and prime
 *            p(n+2)
 * Range:     u8 n <= 10, u16 n <= 325, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

static uint64_t *primes_cache = NULL;
static size_t primes_cache_count = 0;
#define PRIME_LIMIT 200000  /* sufficient for n up to about 10 000 */

static void init_primes(void) {
    if (primes_cache) return;
    size_t cnt;
    build_primes_upto((idx_t)PRIME_LIMIT, &primes_cache, &cnt);
    primes_cache_count = cnt;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    init_primes();
    if (primes_cache_count <= (size_t)(n + 1)) {
        /* If requested n is too large for our precomputed limit,
           extend the sieve (rare for n <= 10 000). */
        size_t new_limit = (size_t)(PRIME_LIMIT * 2);
        while (primes_cache_count <= (size_t)(n + 1) && new_limit < (size_t)(n * 20)) {
            new_limit *= 2;
        }
        uint64_t *new_primes;
        size_t new_cnt;
        build_primes_upto((idx_t)new_limit, &new_primes, &new_cnt);
        free(primes_cache);
        primes_cache = new_primes;
        primes_cache_count = new_cnt;
    }

    idx_t p_n     = primes_cache[n - 1];
    idx_t p_next  = primes_cache[n + 1];  /* p(n+2) */

    seq_t sum = 0;
    for (idx_t val = p_n + 1; val < p_next; ++val) {
        if (!is_prime(val)) {
            sum += (seq_t)val;
        }
    }
    return sum;
}
