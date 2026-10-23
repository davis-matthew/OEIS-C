/*
 * A142209  Primes congruent to 12 mod 41.
 * https://oeis.org/A142209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 166, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 12 mod 41 (OEIS A142209)
   a(n) = the n‑th prime p such that p % 41 == 12  (n ≥ 1) */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Ensure the cache array has at least 'need' slots. */
static void _ensure_capacity(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap > 0) ? (*cap << 1) : 4;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *arr = tmp;
    *cap = new_cap;
}

/* Return the n-th prime of the form 41*k + 12. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* Undefined for non‑positive indices */

    static seq_t *cache = NULL;          /* Cache of already found terms */
    static size_t cache_len = 0;         /* Number of terms stored in cache */
    static size_t cache_cap = 0;         /* Allocated size of cache */

    /* Grow cache until it contains at least n terms. */
    while (cache_len < (size_t)n)
    {
        /* Determine candidate representing the next number in the AP 41*k+12. */
        idx_t candidate;
        if (cache_len == 0)
            candidate = 53;                      /* 41*1 + 12 is the first candidate */
        else
            candidate = (idx_t)cache[cache_len - 1] + 41;

        /* Scan forward until a prime is found. */
        while (!is_prime(candidate))
            candidate += 41;

        /* Store the prime in the cache. */
        _ensure_capacity(&cache, &cache_cap, cache_len + 1);
        cache[cache_len++] = (seq_t)candidate;
    }

    return cache[(size_t)n - 1];
}
