/*
 * A082865  a(n) is the sum of the preceding terms that are coprime to n.
 * https://oeis.org/A082865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 2. For n >= 3, a(n) = sum_{k = 1}^{n-1} a(k)
 *            where gcd(a(k), n) = 1
 * Range:     u8 n <= 14, u16 n <= 32, u32 n <= 60, u64 n <= 112,
 *            u128 n <= 217, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"

/* Cache for already computed terms of the sequence. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* number of allocated slots (max index + 1) */
static idx_t max_computed = 0; /* largest n whose value is in cache */

/* Grow the cache to be able to store index n (and n itself). */
static void ensure_cache_cap(idx_t n)
{
    if ((size_t)(n + 1) <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap <= (size_t)(n + 1))
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cache)
    {
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        free(cache);
    }
    memset(new_cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache       = new_cache;
    cache_cap   = new_cap;
}

/* Compute sequence values up to n (inclusive).  Idempotent. */
static void compute_up_to(idx_t n)
{
    if (max_computed >= n)
        return;

    ensure_cache_cap(n);

    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        if (i == 1)
        {
            cache[i] = (seq_t)1;
            continue;
        }
        if (i == 2)
        {
            cache[i] = (seq_t)2;
            continue;
        }

        seq_t sum = 0;
        seq_t n_seq = (seq_t)i;
        for (idx_t k = 1; k < i; ++k)
        {
            /* gcd(a(k), n) == 1  <=>  gcd(cache[k], n) == 1  */
            if (gcd(cache[k], n_seq) == (seq_t)1)
                sum += cache[k];
        }
        cache[i] = sum;
    }

    max_computed = n;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1 – guard but never expected to be called. */
        return (seq_t)0;
    compute_up_to(n);
    return cache[n];
}
