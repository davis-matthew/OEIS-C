/*
 * A153053  Numbers j such that 2*j + 7 is not a prime.
 * https://oeis.org/A153053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes A153053: the sequence of integers j such that
 * 2*j + 7 is not a prime.  The implementation searches j = 1,2,3,…,
 * testing primality of 2*j+7 with is_prime_big, and stores the found
 * values in a static array so that repeated calls are efficient. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* provides is_prime_big(seq_t) */

/* Forward declaration of is_prime_big */
extern bool is_prime_big(seq_t n);

/* Static cache for previously found terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms currently cached */
static size_t cache_cap = 0;   /* capacity of the cache array */

/* The next j to test (the first j not yet processed) */
static idx_t next_j = 1;

/* Ensure the cache array can hold at least 'need' elements */
static void
ensure_cache_cap(size_t need)
{
    if (need > cache_cap)
    {
        size_t newcap = cache_cap < 8 ? 8 : cache_cap * 2;
        if (newcap < need) newcap = need;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();   /* out of memory – terminate */
        cache = tmp;
        cache_cap = newcap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0 but protects us */

    /* If the requested term is already cached, return it */
    if (n <= cache_len)
        return cache[n-1];

    /* Extend the cache until we have at least n terms */
    while (cache_len < (size_t)n)
    {
        /* Test current j for composite 2*j+7 */
        seq_t val = 2 * (seq_t)next_j + (seq_t)7;
        if (!is_prime_big(val))
        {
            /* Found a term; ensure capacity and store */
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)next_j;
        }
        /* Advance to next candidate j */
        next_j++;
    }

    return cache[n-1];
}
