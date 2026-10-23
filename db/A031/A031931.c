/*
 * A031931  Upper prime of a difference of 12 between consecutive primes.
 * https://oeis.org/A031931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the upper prime of a difference of 12 between
 *            consecutive primes. More precisely, a(n) = p_{i+1} where p_i and
 *            p_{i+1} are consecutive primes with p_{i+1} - p_i = 12
 * Range:     u8 n <= 2, u16 n <= 647, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, build_primes_upto, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    if (n == 0) /* sequence is 1‑indexed; return 0 for safety */
        return (seq_t)0;

    /* Static cache for computed terms */
    static seq_t *cache      = NULL;   /* array of found upper primes */
    static size_t cache_len  = 0;      /* number of terms currently cached */
    static idx_t  limit      = 0;      /* current sieve upper bound */

    /* If we already have the requested term, return it immediately */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    /* We need to grow the cache.  Increase the sieve limit until we have
       at least n terms. */
    idx_t new_limit = (limit == 0) ? 100000 : limit * 2ULL;

    while (1) {
        /* Generate all primes up to the current limit */
        uint64_t *primes   = NULL;
        size_t    primes_cnt = 0;
        build_primes_upto(new_limit, &primes, &primes_cnt);

        /* Count how many gaps of size 12 appear */
        size_t gaps = 0;
        for (size_t i = 1; i < primes_cnt; ++i)
            if (primes[i] - primes[i - 1] == 12)
                ++gaps;

        /* If no gaps found, double the limit and try again */
        if (gaps == 0) {
            free(primes);
            new_limit *= 2ULL;
            continue;
        }

        /* If not enough gaps for the requested n, free current cache (if any)
           and try a larger limit. */
        if (gaps < (size_t)n) {
            if (cache) {
                free_seq_array(cache);
                cache = NULL;
            }
            cache_len = 0;
            limit      = new_limit;
            new_limit *= 2ULL;
            free(primes);
            continue;
        }

        /* We have at least n gaps.  Build a new cache of exact size. */
        seq_t *new_cache = alloc_seq_array(gaps);
        if (!new_cache) {          /* allocation failed – retry with larger limit */
            if (cache) {
                free_seq_array(cache);
                cache = NULL;
            }
            cache_len = 0;
            limit      = new_limit;
            new_limit *= 2ULL;
            free(primes);
            continue;
        }

        /* Fill the cache with the upper primes of the 12‑gap pairs. */
        size_t k = 0;
        for (size_t i = 1; i < primes_cnt && k < gaps; ++i)
            if (primes[i] - primes[i - 1] == 12)
                new_cache[k++] = (seq_t)primes[i];

        /* Clean up the temporary primes array and replace the old cache. */
        free(primes);
        if (cache) {
            free_seq_array(cache);
        }
        cache     = new_cache;
        cache_len = gaps;
        limit      = new_limit;
        break;   /* enough terms have been cached */
    }

    return cache[n - 1];
}
