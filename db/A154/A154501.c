/*
 * A154501  Sum of any 3 consecutive numbers is prime and a(n+2)-(a(n+1)+a(n))
 *          is prime, a(1)=3,a(2)=11.
 * https://oeis.org/A154501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A154501: a(1)=3, a(2)=11.
   For n ≥ 3, a(n) is the smallest prime > a(n-1) such that
        a(n-2)+a(n-1)+a(n)   is prime
        |a(n)-a(n-1)-a(n-2)| is prime
   The terms of the sequence are always prime.
   The routine caches previously computed values for arbitrary
   access order and uses the supplied is_prime_big() helper for
   primality testing on seq_t values.
*/

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Cache for computed terms (1‑based index) */
static seq_t *cache = NULL;          /* array of terms, cache[0] unused      */
static idx_t cache_size = 0;         /* capacity of cache                    */
static idx_t max_computed = 0;       /* largest index whose term is computed */

/* Ensure the cache can hold indices up to 'n' (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;
    idx_t new_size = (n > 64 ? n * 2 : 128);
    seq_t *new_cache = (seq_t*)malloc((new_size + 1) * sizeof(seq_t));
    if (!new_cache)
        abort();                       /* fatal allocation error */
    if (cache) {
        memcpy(new_cache, cache, (cache_size + 1) * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_size = new_size;
}

/* Main sequence function.  Calls are safe in any order. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* offset is 1; undefined for n < 1 */

    /* Initialise base cases lazily. */
    if (max_computed == 0) {
        ensure_cache(2);
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)11;
        max_computed = 2;
    }

    /* If the requested index is already computed, return it. */
    if (n <= max_computed)
        return cache[n];

    /* Expand cache to hold up to 'n'. */
    ensure_cache(n);

    /* Compute terms up to 'n'. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t prev      = cache[i - 1];
        seq_t prev2     = cache[i - 2];
        seq_t sum_ab    = prev + prev2;        /* a(i-1)+a(i-2) */

        seq_t candidate = prev + 1;            /* at least greater than a(i-1) */
        while (1) {
            /* candidate must be prime. */
            if (!is_prime_big(candidate)) {
                ++candidate;
                continue;
            }

            /* sum of the three consecutive terms must be prime. */
            if (!is_prime_big(sum_ab + candidate)) {
                ++candidate;
                continue;
            }

            /* absolute difference must be prime. */
            seq_t diff = (candidate > sum_ab) ? candidate - sum_ab
                                             : sum_ab - candidate;
            if (!is_prime_big(diff)) {
                ++candidate;
                continue;
            }

            /* All conditions satisfied. */
            cache[i] = candidate;
            break;
        }
    }

    max_computed = n;
    return cache[n];
}
