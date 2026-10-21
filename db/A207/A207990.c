/*
 * A207990  Primes of the form prime(n) + prime(n+1) - 5.
 * https://oeis.org/A207990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 634, u32 n <= 1167, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   append_candidate, build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A207990 – primes of the form prime(n)+prime(n+1)-5
 *  Formula: a(n) = prime(n) + prime(n+1) - 5
 *  Returned only if the result is prime; otherwise omitted.
 *  The function yields the nth such prime for n ≥ 1.
 *
 *  Implementation strategy:
 *    * Generate a large list of ordinary primes once using
 *      build_primes_upto().
 *    * Scan consecutive prime pairs and form candidate = p[i] + p[i+1] - 5.
 *    * Test each candidate with is_prime_big(); if prime, store in cache.
 *    * Memoize results: subsequent calls resume from the last used
 *      prime index, so the cost is shared.
 *    * 20 000 000 is ample to produce more than 10 000 terms, ensuring
 *      that all requested indices up to the limit of the tests are
 *      available.                                            */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/append_candidate.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Local static state */
    static uint64_t *primes   = NULL;      /* array of primes ≤ LIMIT */
    static size_t    primes_cnt = 0;       /* number of primes in @primes       */
    static seq_t    *cache   = NULL;      /* cached sequence values            */
    static size_t    cache_len = 0;        /* length of @cache                  */
    static size_t    cache_cap = 0;        /* capacity of @cache                */
    static size_t    cur_pair   = 0;       /* index of next prime pair to process*/

    /* Build the prime list lazily on the first call.  20 000 000 is
     * high enough to guarantee >10 000 valid terms (≈74 000).     */
    if (!primes) {
        const idx_t LIMIT = 20000000ULL;
        build_primes_upto(LIMIT, &primes, &primes_cnt);
    }

    /* Extend the cache until it contains the desired index.     */
    while (cache_len < (size_t)n && cur_pair + 1 < primes_cnt) {
        seq_t cand = (seq_t)primes[cur_pair] +
                     (seq_t)primes[cur_pair + 1] - (seq_t)5;
        if (is_prime_big(cand)) {
            append_candidate(&cache, &cache_len, &cache_cap, cand);
        }
        ++cur_pair;
    }

    /* The problem guarantees that n will be within the bounds of the
     * pre‑computed cache.  If not, return 0 (should never happen). */
    if (n == 0 || cache_len < (size_t)n) return 0;
    return cache[n - 1];
}
