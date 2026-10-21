/*
 * A219023  Number of primes p<n such that n^2-n+p and n^2+n-p are both prime.
 * https://oeis.org/A219023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p < n such that n^2−n+p and n^2+n−p are
 *            both prime. Indexing starts at 1. The implementation uses the
 *            util functions build_primes_upto and is_prime. It caches the
 *            prime list up to the maximum n requested to keep the routine
 *            fast
 * Range:     u8 n <= 15, u16 n <= 261, u32 n <= 9508, u64 n <= 9508,
 *            u128 n <= 9508, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

static uint64_t *prime_cache = NULL;
static size_t prime_cnt = 0;
static idx_t prime_limit = 0;

/* Ensure all primes < limit are available in prime_cache. */
static void ensure_primes_upto(idx_t limit)
{
    if (prime_limit >= limit) return;
    if (prime_cache) free(prime_cache);
    build_primes_upto(limit, &prime_cache, &prime_cnt);
    prime_limit = limit;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    /* Build prime list for the first time or extend it if needed. */
    ensure_primes_upto(n - 1);

    seq_t count = 0;
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;

    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p   = (seq_t)prime_cache[i];
        seq_t w1  = n2 - ns + p;   /* n^2 − n + p */
        seq_t w2  = n2 + ns - p;   /* n^2 + n − p */
        if (is_prime((idx_t)w1) && is_prime((idx_t)w2))
            ++count;
    }
    return count;
}
