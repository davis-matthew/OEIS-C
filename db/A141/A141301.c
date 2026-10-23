/*
 * A141301  Duplicate of A107152.
 * https://oeis.org/A141301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = the n-th prime p with p % 60 == 1 or p % 60 == 49
 * Range:     i8 n <= 2, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   alloc_seq_array, build_primes, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that p ≡ 1 or 49 (mod 60) */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cache = NULL;          /* cached primes meeting the condition */
static size_t cache_len = 0;          /* number of elements currently cached */
static idx_t  sieve_limit = 0;       /* largest number that has been sieved */

/* rebuild the cache up to a new sieve limit */
static void recompute_cache(idx_t new_limit)
{
    idx_t *prime_list = NULL;
    size_t prime_cnt = 0;
    build_primes(new_limit, &prime_list, &prime_cnt);

    /* estimate about 1/8th of the primes satisfy the congruence */
    size_t tmp_cap = prime_cnt / 8 + 10;
    seq_t *tmp = alloc_seq_array(tmp_cap);
    size_t tmp_len = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if ((p % 60 == 1) || (p % 60 == 49)) {
            /* store the prime as seq_t; assume it fits for requested n */
            tmp[tmp_len++] = (seq_t)p;
        }
    }

    free(prime_list);

    /* replace old cache */
    if (cache)
        seq_free_array(cache);
    cache = tmp;
    cache_len = tmp_len;
    sieve_limit = new_limit;
}

/* ensure that cache contains at least n elements (index starts at 1) */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n)
        return;

    idx_t new_limit = (sieve_limit == 0) ? 600 : sieve_limit * 2;
    while (cache_len < n) {
        recompute_cache(new_limit);
        if (cache_len >= n)
            break;
        /* increase the sieve limit for the next attempt */
        new_limit *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n < 1, return 0 as placeholder */
    ensure_cache(n);
    return cache[n - 1];
}
