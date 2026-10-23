/*
 * A037030  Numbers n such that 666*n + 1 is prime.
 * https://oeis.org/A037030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth integer k for which 666*k + 1 is prime. The function
 *            returns the n‑th such k (a(1)=3, a(2)=5, ...)
 * Range:     u8 none, u16 n <= 25, u32 n <= 34, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

/* Static cache for found terms */
static seq_t *cache = NULL;      /* array of k values */
static size_t cache_used = 0;    /* number of terms stored so far */
static size_t cache_cap  = 0;    /* allocated capacity */
static idx_t  next_cand   = 1;   /* next integer to test */

/* Expand the cache if it cannot hold `needed` elements. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < needed) new_cap <<= 1;

    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence.  n ≥ 1. */
seq_t a(idx_t n)
{
    /* Ensure the cache contains the first n terms. */
    while (cache_used < (size_t)n) {
        seq_t val = (seq_t)666 * (seq_t)next_cand + 1;  /* 666*k + 1 */
        if (is_prime_big(val)) {
            grow_cache(cache_used + 1);
            cache[cache_used++] = (seq_t)next_cand;
        }
        ++next_cand;
    }

    return cache[n - 1];
}
