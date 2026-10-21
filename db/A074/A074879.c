/*
 * A074879  10 - Mod(prime(n),10) when prime(n) + 22 = prime(n+1).
 * https://oeis.org/A074879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A074879
   a(n) = 10 - (prime(n) mod 10)
   where prime(n) + 22 = prime(n+1)
   a(1) is the first n for which the condition holds.
*/
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static idx_t  cache_limit = 0;

static void regenerate_cache(idx_t n)
{
    /* If we already have enough entries, nothing to do. */
    if (cache_len >= (size_t)n) return;

    /* Start from a modest limit and double it until we have enough terms. */
    if (cache_limit == 0) cache_limit = 500000;   /* initial guess */

    while (1) {
        uint64_t *primes = NULL;
        size_t   prime_cnt = 0;

        build_primes_upto(cache_limit, &primes, &prime_cnt);

        /* Count the pairs with difference 22. */
        size_t pair_cnt = 0;
        for (size_t i = 0; i + 1 < prime_cnt; ++i)
            if (primes[i + 1] - primes[i] == 22)
                ++pair_cnt;

        /* Allocate (or reallocate) the cache. */
        seq_t *new_cache = (seq_t *)malloc(pair_cnt * sizeof(seq_t));
        size_t idx = 0;
        for (size_t i = 0; i + 1 < prime_cnt && idx < pair_cnt; ++i) {
            if (primes[i + 1] - primes[i] == 22) {
                uint64_t p = primes[i];
                new_cache[idx++] = (seq_t)(10 - (p % 10));
            }
        }

        free(primes);
        free(cache);
        cache      = new_cache;
        cache_len  = pair_cnt;

        if (cache_len >= (size_t)n)
            break;          /* we have enough terms now */

        /* Not enough terms: increase the limit and try again. */
        cache_limit *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* out of bounds, but safe */
    regenerate_cache(n);
    return cache[(size_t)(n - 1)];
}
