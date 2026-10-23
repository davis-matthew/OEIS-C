/*
 * A179445  Nonprimes m such that 10m + 1 are primes.
 * https://oeis.org/A179445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: nonprime m such that 10m + 1 are primes. */
/* Computed by scanning m from 1 upwards and testing primality. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/is_prime.h"       /* bool is_prime(idx_t x); */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */

/* Static cache of computed terms. */
static seq_t *cache = NULL;          /* array of seen terms */
static size_t cache_size = 0;        /* number of terms stored */
static size_t cache_cap   = 0;        /* capacity of the array */
static idx_t last_m = 0;             /* last m that has been examined */

/* Ensure that the cache can hold at least 'needed' elements. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* In a library context, aborting is acceptable. */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* Return cached value if already computed. */
    if (cache && cache_size >= n)
        return cache[n - 1];

    /* Generate more terms until we have at least n of them. */
    while (cache_size < n) {
        last_m++;                     /* Next candidate m. */

        /* Skip prime m; we need non‑prime m (including 1). */
        if (is_prime(last_m))
            continue;

        /* Compute 10*m + 1. Use uint64_t as an intermediate
           to avoid overflow for moderate m. */
        uint64_t p_val = 10ULL * (uint64_t)last_m + 1ULL;
        seq_t p = (seq_t)p_val;

        /* Test whether 10*m + 1 is prime. */
        if (!is_prime_big(p))
            continue;

        /* Valid term found: store it. */
        ensure_cache_cap(cache_size + 1);
        cache[cache_size++] = (seq_t)last_m;
    }

    return cache[n - 1];
}
