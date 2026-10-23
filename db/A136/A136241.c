/*
 * A136241  Numbers n among A006093 such that n^2 + n - 1 is prime.
 * https://oeis.org/A136241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A091567(n) - 1 (Numbers n such that n+1 is prime and n^2
 *            + n - 1 is prime.) The code searches through the integers, tests
 *            each prime p for the additional primality condition p^2 - p - 1,
 *            and records n = p-1
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 20, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;      /* array of sequence terms n */
static size_t cache_len = 0;     /* number of terms currently stored */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static idx_t next_int = 1;       /* next integer to test (starts at 2) */

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* unrecoverable error */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1 */
    /* Extend the cache until it contains at least n terms. */
    while (cache_len < n) {
        ++next_int;                /* try integer next_int as candidate p */
        seq_t p = (seq_t)next_int; /* p must be prime (p = n+1) */
        if (!is_prime_big(p)) continue;  /* p not prime, skip */
        seq_t val = p * p - p - (seq_t)1; /* compute p^2 - p - 1 */
        if (!is_prime_big(val)) continue; /* extra condition fails */
        /* p satisfies both conditions -> add n = p-1 to cache */
        ensure_cache_capacity(cache_len + 1);
        cache[cache_len++] = p - (seq_t)1;
    }
    return cache[n - 1];           /* 1‑based indexing */
}
