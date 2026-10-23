/*
 * A173001  Values of n such that exactly one of 5n+1 and 5n^2+5n+1 is prime.
 * https://oeis.org/A173001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 49, u32 n <= 56, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A173001.
   a(n) is the n-th natural number m such that exactly one of
   5*m + 1 and 5*m² + 5*m + 1 is prime. */
#include <stdbool.h>
#include <stdlib.h>

/* Prime test provided by the utils. */
#include "../utils/is_prime_big.h"

/* Static cache of already found terms. */
static seq_t *cache = NULL;      /* array of terms */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t next_test = 0;      /* next natural number to examine */

/* Resizes the cache if necessary. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                    /* Out of memory */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialize cache on first use. */
    if (!cache)
    {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();                /* Out of memory */
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < n)
    {
        ++next_test;                   /* m = next natural number to test */
        seq_t m = (seq_t)next_test;

        /* Compute 5*m + 1 */
        seq_t f1 = (seq_t)5 * m + (seq_t)1;

        /* Compute 5*m^2 + 5*m + 1 */
        seq_t f2 = (seq_t)5 * m * m + (seq_t)5 * m + (seq_t)1;

        bool p1 = is_prime_big(f1);
        bool p2 = is_prime_big(f2);

        if (p1 ^ p2)                   /* exactly one is prime */
        {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = m;
        }
    }

    return cache[n - 1];                /* a(1) is the first element */
}
