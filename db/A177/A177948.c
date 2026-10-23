/*
 * A177948  Numbers n such that n^2 + reverse of n^2 is a prime.
 * https://oeis.org/A177948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_prime_big, reverse_decimal, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find numbers n such that n² + reverse(n²) is a prime. */

#include <stddef.h>
#include <stdbool.h>

#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/is_prime_big.h"
#include "../utils/reverse_decimal.h"

/* Global cache to store the terms of the sequence. */
static seq_t *cache   = NULL;   /* pointer to cached seq_t values */
static size_t cache_size = 0;    /* number of terms currently stored */
static size_t cache_cap  = 0;    /* allocated capacity of the cache */

/* Ensure the cache has room for at least `need` terms. */
static void
ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    while (new_cap < need)
        new_cap <<= 1;

    if (cache_cap == 0)
        cache = seq_alloc(new_cap);
    else
        cache = seq_realloc(cache, cache_cap, new_cap);

    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;            /* undefined for indices < 1 */

    /* Expand the cache until it contains the desired term. */
    if (cache_size < (size_t)n) {
        ensure_cache_cap((size_t)n);

        /* Start searching from the smallest integer that has not yet been
           considered.  The first term is 1. */
        idx_t cand = (cache_size == 0) ? 1
                                        : (idx_t)cache[cache_size - 1] + 1;

        for (; cache_size < (size_t)n; ++cand) {
            seq_t sq = (seq_t)cand;
            sq = sq * sq;                    /* n² */

            seq_t rev_sq = reverse_decimal(sq);  /* reverse(n²) */
            seq_t sum    = sq + rev_sq;         /* n² + reverse(n²) */

            if (is_prime_big(sum))
                cache[cache_size++] = (seq_t)cand;  /* store the term */
        }
    }

    return cache[(size_t)n - 1];
}
