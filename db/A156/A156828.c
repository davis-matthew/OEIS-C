/*
 * A156828  a(1) = 2. a(n) = the smallest prime >= a(n-1) + 4.
 * https://oeis.org/A156828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; a(n) = the smallest prime ≥ a(n-1)+4 for n>1
 * Range:     u8 n <= 37, u16 n <= 4530, u32 n <= 5498, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>      /* realloc, abort */
#include <stdbool.h>     /* bool, false, true */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t) */

/* Cached values of the sequence.  cache[0] = a(1), cache[1] = a(2), ... */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of filled entries */
static size_t cache_cap = 0;   /* allocated capacity */

/* Ensure cache contains at least n entries (a(1)…a(n)) */
static void ensure_cache(idx_t n)
{
    /* nothing to do for invalid indices */
    if (n < 1)
        return;

    /* allocate or grow the array */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)n)
            new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* unrecoverable failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* fill missing terms */
    if (cache_len == 0) {
        cache[0] = (seq_t)2;          /* a(1) = 2 */
        cache_len = 1;
    }
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        seq_t candidate = cache[i - 1] + (seq_t)4;
        while (!is_prime_big(candidate))
            candidate++;
        cache[i] = candidate;
    }
    cache_len = (size_t)n;
}

/* Return the nth term of the sequence.  The function is safe to call
   in any order of n.  For n<=0 it simply returns 0. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
