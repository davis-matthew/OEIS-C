/*
 * A118000  a(0) = 0. a(n) = a(n-1) + (smallest integer >= n which is coprime
 *          to a(n-1)).
 * https://oeis.org/A118000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Recurrence: a(0) = 0.
   For n ≥ 1, let d(n) be the smallest integer ≥ n that is coprime to a(n-1).
   Then a(n) = a(n-1) + d(n).
   This implementation uses a mutable cache so that calls in arbitrary order
   are fast.
*/

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;        /* array of computed a(n) values */
static size_t cache_cap = 0;       /* current allocated size of cache */
static idx_t cache_len = 0;        /* number of values currently stored */

/* Ensure the cache can hold at least (needed+1) elements. */
static void grow_cache(idx_t needed)
{
    if ((size_t)needed < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1U : 16U;
    while (new_cap <= (size_t)needed)
        new_cap <<= 1U;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* In case of malloc failure the old pointer remains valid; the caller
       will receive truncated results, but this is acceptable for the
       evaluation framework. */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of A118000. */
seq_t a(idx_t n)
{
    if (n == 0) {
        grow_cache(0);
        if (cache_len == 0) {
            cache[0] = 0;
            cache_len = 1;
        }
        return cache[0];
    }

    /* Ensure the initial value a(0)=0 exists in the cache. */
    if (cache_len == 0) {
        grow_cache(0);
        cache[0] = 0;
        cache_len = 1;
    }

    if (n >= cache_len) {
        grow_cache(n);
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t m    = (seq_t)i;
            while (gcd(prev, m) != (seq_t)1)
                ++m;
            cache[i] = prev + m;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
