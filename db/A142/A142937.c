/*
 * A142937  Primes congruent to 27 mod 64.
 * https://oeis.org/A142937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime p such that p ≡ 27 (mod 64)
 * Range:     u8 none, u16 n <= 200, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"   /* provides bool is_prime_small(idx_t x) */

/* cache for previously computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms currently cached   */
static size_t cache_cap = 0;   /* allocated capacity of the cache    */
static idx_t next_k = 1;       /* next k such that number = 64*k + 27 */

/* ensure that cache has at least 'needed' slots */
static void ensure_cache_size(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) /* the sequence is 1‑indexed; behaviour below offset is irrelevant */
        return 0;

    /* return cached value if already present */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* extend cache to contain up to n terms */
    ensure_cache_size((size_t)n);

    /* compute missing terms until we reach n */
    while (cache_len < (size_t)n)
    {
        /* find next prime of the form 64*k+27 */
        while (true)
        {
            idx_t candidate = 64 * next_k + 27;
            if (is_prime_small(candidate))
                break;           /* prime found */
            ++next_k;           /* try next candidate */
        }

        /* store the found prime */
        cache[cache_len] = (seq_t)(64 * next_k + 27);
        ++cache_len;
        ++next_k;                /* next search starts after this candidate */
    }

    return cache[n - 1];
}
