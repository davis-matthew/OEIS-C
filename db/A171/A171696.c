/*
 * A171696  Nonnegative numbers k such that neither of 6*k +- 1 is prime.
 * https://oeis.org/A171696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of A171696:  
   The sequence of nonnegative integers k such that neither 6*k-1 nor 6*k+1 is prime.
   The term a(n) (n≥1) is the (n‑1)th element stored in the dynamic cache.
   On first use the cache is initially empty; subsequent calls reuse previously
   computed values.  The function grows the cache as needed.  All primes are
   tested with the provided is_prime(…) routine. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *cache  = NULL;   /* dynamic array holding sequence values   */
static idx_t  cache_len = 0;   /* number of values stored              */
static size_t cache_cap = 0;   /* capacity of cache array               */
static uint64_t next_k = 0;    /* next candidate k to test              */

/* Ensure that the cache can hold at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!newp) abort();        /* allocation failure */
    cache = newp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined offset; return 0 for safety */

    /* Return cached value if it already exists. */
    if (n <= cache_len)
        return cache[n-1];

    /* Generate terms until we have the requested nth element. */
    uint64_t k;
    for (k = next_k; cache_len < n; ++k) {
        uint64_t f = 6*k - 1;
        uint64_t g = 6*k + 1;
        if (!is_prime(f) && !is_prime(g)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)k;
        }
    }

    /* Set the next candidate for future calls. */
    next_k = k;

    return cache[n-1];
}
