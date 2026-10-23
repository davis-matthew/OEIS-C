/*
 * A048097  Numbers k such that k^2 + k + 11 is prime.
 * https://oeis.org/A048097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   k such that k^2 + k + 11 is prime
 * Range:     u8 n <= 13, u16 n <= 114, u32 n <= 114, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (a(n) returns the nth such k, with a(1) = 0) */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Cache for already found terms */
static seq_t *cache      = NULL;   /* array of k-values */
static size_t cache_cap  = 0;      /* allocated capacity */
static size_t cache_size = 0;      /* number of valid entries */
static idx_t  next_k     = 0;      /* next candidate k to test */

/* Ensure there is room for at least 'need' elements */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap < need)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < need)
            new_cap *= 2;

        /* Allocate new block */
        seq_t *new_buf = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_buf)
            abort();

        /* Copy existing elements */
        for (size_t i = 0; i < cache_size; ++i)
            new_buf[i] = cache[i];

        free(cache);
        cache = new_buf;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* n >= 1 by specification; behaviour for n < 1 is irrelevant */
    if (n == 0)
        return 0;

    /* Grow cache until it contains at least n terms */
    while (cache_size < (size_t)n)
    {
        /* Compute k^2 + k + 11 */
        seq_t kk  = (seq_t)next_k;
        seq_t val = kk * kk + kk + (seq_t)11;

        /* Test primality */
        if (is_prime_big(val))
        {
            ensure_cache_cap(cache_size + 1);
            cache[cache_size++] = next_k;
        }
        ++next_k;
    }
    return cache[(size_t)n - 1];
}
