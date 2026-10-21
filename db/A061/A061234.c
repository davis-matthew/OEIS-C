/*
 * A061234  Smallest number with prime(n)^2 divisors where prime(n) is the
 *          n-th prime.
 * https://oeis.org/A061234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6^(prime(n)-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow, build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/build_primes.h"

static idx_t *prime_cache = NULL;
static size_t prime_cnt   = 0;
static idx_t prime_limit  = 0;

/* Ensure that prime_cache contains at least n primes (prime(n) exists). */
static void ensure_primes(idx_t n) {
    if (prime_cnt >= n) return;
    idx_t limit = prime_limit > 0 ? prime_limit : 100;
    while (prime_cnt < n) {
        /* Increase the sieve limit until we have enough primes. */
        limit = (limit < 2 ? 2 : limit * 2);
        idx_t *new_cache = NULL;
        size_t new_cnt   = 0;
        build_primes(limit, &new_cache, &new_cnt);
        free(prime_cache);
        prime_cache = new_cache;
        prime_cnt   = new_cnt;
        prime_limit = limit;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* value for out-of-range indices */
    ensure_primes(n);
    idx_t p = prime_cache[n-1];
    return big_pow((seq_t)6, (seq_t)(p - 1));
}
