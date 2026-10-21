/*
 * A086321  Primes produced by repeated application of the formula p -> (6p +-
 *          5) starting at the prime 2.
 * https://oeis.org/A086321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 25, u32 n <= 26, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A086321: primes obtained by repeated application of the mapping
 *    p -> 6p ± 5  starting from 2.
 * The sequence is generated in breadth‑first order, i.e. the children
 * of a prime are appended to the end of the list and the list is
 * processed left to right.  The function memoises the values produced
 * so that any order of calls to a(idx_t) is supported with a fast
 * lookup for the nth term. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;       /* stored sequence values      */
static size_t cache_cap = 0;      /* current allocated capacity  */
static size_t cache_size = 0;     /* number of values stored     */
static size_t cache_proc = 0;     /* next element to expand from */

static void ensure_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;

    /* initialise on first use */
    if (!cache)
    {
        ensure_cap(1);
        cache[0] = (seq_t)2;
        cache_size = 1;
        cache_proc = 0;
    }

    /* grow the sequence until it contains at least n elements */
    while (cache_size < n)
    {
        /* Safety: only try to expand a node that exists */
        seq_t p = cache[cache_proc++];
        seq_t c1 = (seq_t)6 * p - (seq_t)5;
        if (is_prime_big(c1))
        {
            ensure_cap(cache_size + 1);
            cache[cache_size++] = c1;
        }
        seq_t c2 = (seq_t)6 * p + (seq_t)5;
        if (is_prime_big(c2))
        {
            ensure_cap(cache_size + 1);
            cache[cache_size++] = c2;
        }
    }

    return cache[n - 1];
}
