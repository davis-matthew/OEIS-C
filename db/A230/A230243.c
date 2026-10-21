/*
 * A230243  Number of primes p < n with 3*p + 8 and (p-1)*n + 1 both prime.
 * https://oeis.org/A230243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p < n such that 3p + 8 and (p-1)*n + 1
 *            are both prime
 * Range:     u8 n <= 20, u16 n <= 1304, u32 n <= 1304, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_big.h"

/* Global cache for primes up to the maximum limit requested so far. */
static uint64_t *prime_cache = NULL;
static size_t prime_cache_cnt = 0;
static idx_t prime_cache_limit = 0;

/* Ensure that prime_cache contains all primes ≤ limit. */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_cache_limit)
        return;                             /* Already up to date. */

    /* Release previous cache. */
    if (prime_cache)
        free(prime_cache);

    /* Build new primes up to the requested limit. */
    uint64_t *tmp = NULL;
    size_t tmp_cnt = 0;
    build_primes_upto(limit, &tmp, &tmp_cnt);

    prime_cache = tmp;
    prime_cache_cnt = tmp_cnt;
    prime_cache_limit = limit;
}

seq_t a(idx_t n)
{
    if (n <= 2)            /* No primes < n when n ≤ 2 */
        return 0;

    idx_t limit = n - 1;
    ensure_primes_upto(limit);

    seq_t count = 0;
    seq_t seq_n = (seq_t)n;

    for (size_t i = 0; i < prime_cache_cnt; ++i) {
        uint64_t p = prime_cache[i];
        if ((seq_t)p >= n)   /* All subsequent primes are ≥ n */
            break;

        seq_t val1 = (seq_t)3 * (seq_t)p + (seq_t)8;
        if (!is_prime_big(val1))
            continue;

        seq_t val2 = ((seq_t)p - (seq_t)1) * seq_n + (seq_t)1;
        if (!is_prime_big(val2))
            continue;

        count++;
    }

    return count;
}
