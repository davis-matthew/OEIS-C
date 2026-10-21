/*
 * A051899  Smallest prime p such that (p-1)/(2*n) is also a prime.
 * https://oeis.org/A051899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest prime p such that (p-1)/(2*n) is prime
 * Range:     u8 n <= 19, u16 n <= 325, u32 n <= 6689, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"

/* Memoization cache for computed sequence values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

static void ensure_cache_capacity(idx_t n)
{
    if (n <= cache_cap)
        return;
    size_t new_cap = n < 8 ? 8 : n * 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (new_cache == NULL)
        abort(); /* Out of memory; caller cannot recover */
    for (size_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = (seq_t)0;
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* Undefined for n = 0; return 0 as a safe value */

    /* Ensure cache has enough space for index n */
    ensure_cache_capacity(n);

    /* Return cached value if already computed */
    if (cache_len >= n && cache[n - 1] != (seq_t)0)
        return cache[n - 1];

    seq_t result = (seq_t)0;
    uint64_t k = 2;
    while (1)
    {
        if (is_prime_small(k))
        {
            seq_t p = (seq_t)1 + (seq_t)2 * (seq_t)n * (seq_t)k;
            if (is_prime_big(p))
            {
                result = p;
                break;
            }
        }
        ++k;
    }

    /* Store result in cache */
    cache[n - 1] = result;
    if (cache_len < n)
        cache_len = n;

    return result;
}
