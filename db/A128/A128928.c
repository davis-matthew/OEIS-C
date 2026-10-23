/*
 * A128928  Smallest member p of a triple of primes (p,p+8,p+20).
 * https://oeis.org/A128928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the smallest prime p such that (p, p+8, p+20) are all prime.
   a(1)=3, a(2)=11, ... */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utilities that are provided by the build system. */
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"

/* static cache of already discovered terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms stored */
static size_t cache_cap = 0;   /* allocated capacity */

/* next prime to start testing after previously accepted prime */
static uint64_t next_start = 2;

/* enlarge cache if necessary */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = needed * 2 + 16;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* If realloc fails we simply keep the old cache; the program
       will keep retrying until memory is available or the caller
       has reached the end of the sequence. */
    if (tmp)
    {
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* fill the cache up to index n (1‑based) */
static void grow_cache_to(idx_t n)
{
    if ((size_t)n <= cache_len)
        return;

    while ((size_t)n > cache_len)
    {
        /* get next candidate prime */
        uint64_t p = next_prime(next_start);
        next_start = p + 1;   /* next search starts after this prime */

        /* test the triple (p, p+8, p+20) */
        if (is_prime((idx_t)p + 8) && is_prime((idx_t)p + 20))
        {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len] = (seq_t)p;
            cache_len++;
        }
    }
}

/* public entry point */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n=0 – offset starts at 1 */

    grow_cache_to(n);
    return cache[n - 1];
}
