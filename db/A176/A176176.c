/*
 * A176176  Numbers k such that 2^(k-1) == 4^(k-1) (mod k).
 * https://oeis.org/A176176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 6645, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) of OEIS A176176: numbers k such that 2^(k-1) ≡ 4^(k-1) (mod k) */
/* The implementation caches found terms so that any call order is O(1) on the cached part. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

/* Helper to freely realloc in both C and C++ environments. */
static inline seq_t *seq_realloc(void *ptr, size_t n)
{
#if defined(__cplusplus)
    return static_cast<seq_t*>(realloc(ptr, n));
#else
    return (seq_t*)realloc(ptr, n);
#endif
}

static seq_t *cache      = NULL;   /* storage for found terms          */
static size_t cache_size = 0;      /* number of terms actually stored   */
static size_t cache_cap  = 0;      /* allocated capacity of cache       */
static idx_t  next_k     = 1;      /* next natural number to test       */

/* Ensure the cache can hold `needed` terms. */
static void grow_cache(size_t needed)
{
    if (cache_cap < needed)
    {
        size_t new_cap = (cache_cap > 0) ? cache_cap : 8;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = seq_realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Extend the cache until it contains at least `upto` terms. */
static void fill_cache(size_t upto)
{
    grow_cache(upto);
    while (cache_size < upto)
    {
        /* test whether next_k satisfies the congruence */
        seq_t a = mod_pow((seq_t)2, (uint64_t)next_k - 1, (seq_t)next_k);
        seq_t b = mod_pow((seq_t)4, (uint64_t)next_k - 1, (seq_t)next_k);
        if (a == b) cache[cache_size++] = (seq_t)next_k;
        ++next_k;
    }
}

/* Return the n-th term (n ≥ 1) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 */
    fill_cache((size_t)n);
    return cache[n - 1];
}
