/*
 * A120222  Numbers k such that 6+k and 6*k+1 are prime.
 * https://oeis.org/A120222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 2351, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS: A120222
    Numbers k such that 6 + k and 6*k + 1 are prime.
    Implementation: incremental search with memoised cache.
*/

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, abort */
#include <stdbool.h>  /* bool */

#include "../utils/is_prime.h"   /* bool is_prime(idx_t) */

/* static cache for already found terms */
static seq_t *cache      = NULL;   /* array of results */
static size_t cache_size = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* allocated capacity */
static idx_t  next_k     = 1;      /* next candidate k to test */

/* Ensure the cache can hold at least one more entry. */
static void grow_cache_if_needed(void)
{
    if (cache_size >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* out of memory */
        cache      = tmp;
        cache_cap  = new_cap;
    }
}

/* Find the next k satisfying the prime conditions and store it. */
static void find_next_term(void)
{
    idx_t k = next_k;
    for (;;) {
        idx_t a = k + 6;
        idx_t b = 6 * k + 1;
        if (is_prime(a) && is_prime(b))
            break;
        ++k;
    }
    grow_cache_if_needed();
    cache[cache_size++] = (seq_t)k;
    next_k = k + 1;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    /* n is 1‑based; return the n-th term. */
    if (n == 0) return 0;        /* undefined, but avoid UB */
    while (cache_size < (size_t)n)
        find_next_term();
    return cache[n - 1];
}
