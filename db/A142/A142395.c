/*
 * A142395  Primes congruent to 44 mod 47.
 * https://oeis.org/A142395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime p such that p % 47 == 44
 * Range:     u8 none, u16 n <= 137, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* abort */
#include <stdint.h>   /* uint64_t, size_t */
#include <stdbool.h>
#include "../utils/is_prime.h"   /* is_prime(idx_t) */
#include "../utils/seq_realloc.h"/* seq_realloc(seq_t*,size_t,size_t) */

static seq_t *cache   = NULL;  /* cached primes */
static size_t cache_cap = 0;   /* allocated capacity */
static size_t cache_sz  = 0;   /* number of cached primes */
static uint64_t candidate = 44;/* last checked number */

/* Ensure the cache has at least `need` elements; reallocate if necessary. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    seq_t *new_cache = seq_realloc(cache, cache_cap, need);
    if (!new_cache)
        abort();           /* allocation failed; terminate to avoid UB */
    cache = new_cache;
    cache_cap = need;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n == 0; return 0 by convention */
        return (seq_t)0;

    /* Generate primes until we have at least `n` stored. */
    while (cache_sz < (size_t)n)
    {
        candidate += 47ULL;          /* next number ≡ 44 (mod 47) */
        if (is_prime(candidate))
        {
            seq_t val = (seq_t)candidate;
            ensure_cache_cap(cache_sz + 1);
            cache[cache_sz++] = val;
        }
    }
    return cache[n - 1];             /* zero‑based index into cached list */
}
