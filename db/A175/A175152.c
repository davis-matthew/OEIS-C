/*
 * A175152  Numbers k such that 11*k-1 and 11*k+1 are twin primes.
 * https://oeis.org/A175152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k such that 11*k-1 and 11*k+1 are twin primes
 * Range:     u8 n <= 8, u16 n <= 680, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;          /* stored k values */
static size_t cache_len = 0;         /* number of values stored */
static size_t cache_cap = 0;         /* capacity of cache array */
static idx_t next_k = 1;             /* next k to test */

/* Ensure the cache has space for at least need elements. */
static void ensure_cache_size(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed – terminate program. */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Return true iff 11*k-1 and 11*k+1 are both prime. */
static inline bool twin_primes(idx_t k)
{
    idx_t m = 11 * k;
    return is_prime(m - 1) && is_prime(m + 1);
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* defined only for n >= 1 */

    /* Extend cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        /* Find next k with the twin‑prime property. */
        while (!twin_primes(next_k))
            ++next_k;

        /* Store the found k. */
        ensure_cache_size(cache_len + 1);
        cache[cache_len++] = (seq_t)next_k;
        ++next_k;
    }
    return cache[(size_t)n - 1];
}
