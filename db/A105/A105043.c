/*
 * A105043  Numbers n such that 100*n - 1 is prime.
 * https://oeis.org/A105043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: returns the n-th positive integer m such that 100*m-1 is prime */
/* a(1)=2, a(2)=5, a(3)=6, a(4)=14, a(5)=15, … */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_uint64.h"

/* cache of already-found terms */
static seq_t *cache = NULL;      /* array of terms */
static idx_t  cache_cap = 0;     /* allocated capacity */
static idx_t  cache_size = 0;    /* number of terms stored */

/* next candidate integer to test */
static idx_t next_candidate = 1;

/* ensure the cache can hold at least 'need' items */
static void ensure_cache(idx_t need)
{
    if (need <= cache_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);

    cache = tmp;
    cache_cap = new_cap;
}

/* external function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0)                   /* offset 1: a(1) is the first term */
        return 0;

    /* generate terms until we have at least n of them */
    while (cache_size < n) {
        /* find next m such that 100*m-1 is prime */
        uint64_t candidate = (uint64_t)next_candidate;
        uint64_t p = 100ULL * candidate - 1ULL;

        while (!is_prime_uint64(p)) {
            ++candidate;
            p = 100ULL * candidate - 1ULL;
        }

        /* store the new term */
        ensure_cache(cache_size + 1);
        cache[cache_size] = (seq_t)candidate;
        ++cache_size;

        /* advance next_candidate past the prime we just found */
        next_candidate = candidate + 1;
    }

    return cache[n - 1];
}
