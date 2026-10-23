/*
 * A124379  Numbers n such that 120n-1, 120n+1 are twin primes.
 * https://oeis.org/A124379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 49, u32 n <= 56, u64 n <= 5746,
 *            u128 n <= 5746, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A124379: numbers n such that 120n-1 and 120n+1 are twin primes. */
/* The sequence is defined for n ≥ 1; a(1)=2, a(2)=5, ... as listed on OEIS. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Static cache for memoised terms (1‑indexed). */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static idx_t  cache_sz  = 0;

/* Next n to examine for the property. */
static idx_t next_k = 1;

/* Ensure the cache can hold at least |need| elements. */
static void grow_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        return; /* allocation failure – future calls will silently fail */
    cache = tmp;
    cache_cap = new_cap;
}

/* Test whether 120*k-1 and 120*k+1 are both prime. */
static bool candidate(idx_t k)
{
    seq_t mult = (seq_t)120 * (seq_t)k;
    seq_t a = mult - (seq_t)1;
    seq_t b = mult + (seq_t)1;
    return is_prime_big(a) && is_prime_big(b);
}

/* Return the n‑th element of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined, but kept for type consistency */

    /* Generate terms until we have at least n entries. */
    while (cache_sz < n) {
        if (candidate(next_k)) {
            grow_cache(cache_sz + 1);
            cache[cache_sz++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
