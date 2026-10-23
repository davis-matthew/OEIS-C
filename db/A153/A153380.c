/*
 * A153380  Numbers k such that 10*k+9 is not prime.
 * https://oeis.org/A153380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n‑th non‑negative integer k such that 10*k+9 is composite (not prime). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helpers for primality testing and dynamic array reallocation. */
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

/* Cache for generated terms. */
static seq_t *cache      = NULL;   /* array of a(1)..a(cache_cap) */
static size_t cache_size = 0;      /* number of cached terms */
static size_t cache_cap  = 0;      /* allocated capacity */

/* Next candidate k (>= cache_size). */
static seq_t next_k = 0;

/* Ensure that the cache contains at least n terms. */
static void
ensure_cache(idx_t n)
{
    /* Grow the cache array if needed. */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)n)
            new_cap = n;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* Generate terms until we have n cached entries. */
    while (cache_size < (size_t)n) {
        seq_t cand = (seq_t)10 * next_k + (seq_t)9;
        if (!is_prime_big(cand))
            cache[cache_size++] = next_k;
        ++next_k;
    }
}

/* Public function required by the build system. */
seq_t
a(idx_t n)
{
    if (n < 1)
        abort();               /* sequence is 1‑based; out‑of‑range is an error */
    ensure_cache(n);
    return cache[n - 1];
}
