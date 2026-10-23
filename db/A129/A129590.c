/*
 * A129590  Numbers k such that prime(k) + 2*(k-1)*prime(k-1) is prime.
 * https://oeis.org/A129590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 27, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   get_prime, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that prime(k) + 2·(k−1)·prime(k−1) is prime.
   a(n) returns the n‑th such k (offset 1). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/isPrimeGeneric.h"

#ifdef __cplusplus
#include <vector>
/* C++ implementation – use std::vector for automatic object handling */
static std::vector<seq_t> cache;
static idx_t last_k = 0;

/* Prepare the cache so that it contains at least `need` terms. */
static void ensure_cache(idx_t need)
{
    if (cache.size() >= (size_t)need)
        return;

    cache.reserve(need);
    while (cache.size() < (size_t)need)
    {
        ++last_k;
        seq_t pk = get_prime(last_k);
        seq_t expr = pk;
        if (last_k > 1)
        {
            expr += (seq_t)2 * ((seq_t)(last_k - 1)) * get_prime(last_k - 1);
        }
        if (isPrimeGeneric(expr))
            cache.push_back(last_k);
    }
}

/* Return the n‑th element of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n<1 */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
#else
/* C implementation – store results in a dynamically resized array. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_k = 0;

/* Grow the cache to hold at least `need` terms. */
static void ensure_cache(idx_t need)
{
    if (cache_len >= (size_t)need)
        return;

    if (!cache)
    {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(*cache));
        if (!cache)
            abort();
    }
    else if (cache_len >= cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(*cache));
        if (!tmp)
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    while ((size_t)cache_len < (size_t)need)
    {
        ++last_k;
        seq_t pk = get_prime(last_k);
        seq_t expr = pk;
        if (last_k > 1)
        {
            expr += (seq_t)2 * ((seq_t)(last_k - 1)) * get_prime(last_k - 1);
        }
        if (isPrimeGeneric(expr))
        {
            cache[cache_len++] = (seq_t)last_k;
            if (cache_len >= cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(*cache));
                if (!tmp)
                    abort();
                cache = tmp;
                cache_cap = new_cap;
            }
        }
    }
}

/* Return the n‑th element of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n<1 */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
#endif
