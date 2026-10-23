/*
 * A142103  Primes congruent to 11 mod 36.
 * https://oeis.org/A142103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 418, u32 n <= 463, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime congruent to 11 (mod 36).  The sequence
   is unbounded; the first term is a(1)=11. */

#include <stdlib.h>               /* realloc */
#include "../utils/is_prime_big.h"

/* Static cache for already found terms.  */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Next candidate to test, always ≡ 11 (mod 36).  */
static idx_t next_candidate = 11;

/* Append v to the cache, expanding the array if necessary.  */
static void cache_append(seq_t v)
{
    if (cache_len == cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = v;
}

/* Return the n‑th prime congruent to 11 modulo 36.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* out of range, harmless */

    /* Generate terms until the cache contains at least n elements.  */
    while (cache_len < n)
    {
        if (is_prime_big((seq_t)next_candidate))
            cache_append((seq_t)next_candidate);
        next_candidate += 36;           /* keeps the property ≡ 11 (mod 36) */
    }
    return cache[n - 1];
}
