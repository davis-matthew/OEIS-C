/*
 * A089188  Lesser member p of a pair of twin primes such that p-1 is
 *          squarefree.
 * https://oeis.org/A089188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth lesser member of a twin prime pair (p, p+2) *
 *            with p-1 squarefree (OEIS A089188)
 * Range:     u8 n <= 9, u16 n <= 381, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/is_squarefree.h"

/* Static cache: holds all terms computed so far.
 * cache_len is the number of stored terms, cache_cap is the allocated size. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Grow the cache array to hold at least *needed* terms. */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;                     /* undefined for n==0, nothing to do */

    /* If the cache is empty, seed it with the first term. */
    if (cache_len == 0) {
        cache_cap = 16;                     /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();                /* out of memory */
        cache[0] = (seq_t)3;                /* a(1) = 3 */
        cache_len = 1;
    }

    /* Expand the cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        seq_t last = cache[cache_len - 1];
        seq_t candidate = last + 2;          /* next odd candidate after last */

        /* While we haven't found the next valid twin prime pair: */
        for (;;) {
            /* Check primality of p and p+2, and squarefree condition on p-1. */
            if (is_prime((idx_t)candidate) &&
                is_prime((idx_t)(candidate + 2ULL)) &&
                is_squarefree(candidate - 1ULL)) {

                /* Append this term to the cache. */
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!tmp) abort();        /* out of memory */
                    cache = tmp;
                }
                cache[cache_len++] = candidate;
                break;
            }
            candidate += 2ULL;                /* skip to the next odd number */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0 */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
