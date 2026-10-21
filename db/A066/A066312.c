/*
 * A066312  Numbers, other than prime powers, whose distinct prime factors are
 *          consecutive primes.
 * https://oeis.org/A066312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 491, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, next_prime, seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation for OEIS A066312: Numbers whose distinct prime factors are consecutive primes
   (the square‑free kernel has at least two consecutive primes).  */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"

/* Cached sequence values.  */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* The integer that has been checked last.  */
static uint64_t last_checked = 1;

/* Helper: test whether a 64‑bit integer satisfies the definition.  */
static bool is_valid_uint64(uint64_t m)
{
    /* Smallest possible term is 6.  */
    if (m < 6)
        return false;

    /* Factorization storage – plenty of room.  */
    uint64_t primes[64];
    uint64_t exps[64];   /* Not used, only needed by the helper. */
    size_t   cnt = 0;

    factor_u64(m, primes, exps, &cnt);

    /* Must have at least two distinct primes.  */
    if (cnt < 2)
        return false;

    /* Check consecutiveness of the distinct primes.  */
    for (size_t i = 0; i + 1 < cnt; ++i)
        if (next_prime(primes[i]) != primes[i + 1])
            return false;

    return true;
}

/* Grow the cache to at least a given capacity.  */
static void grow_cache(size_t min_cap)
{
    if (cache_cap >= min_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    if (new_cap < min_cap)
        new_cap = min_cap;
    seq_t *newptr = seq_realloc(cache, cache_cap, new_cap);
    cache = newptr;
    cache_cap = new_cap;
}

/* Generate terms until we have at least n of them cached.  */
static void generate_to(idx_t n)
{
    if (cache_size >= (size_t)n)
        return;

    /* Ensure the cache can hold the new terms.  */
    grow_cache((size_t)n);

    while (cache_size < (size_t)n) {
        ++last_checked;
        if (is_valid_uint64(last_checked)) {
            cache[cache_size++] = last_checked;
        }
    }
}

/* Main exported function.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* Undefined for n=0; return 0 just in case. */
    generate_to(n);
    return cache[n - 1];
}
