/*
 * A145154  Coefficients in expansion of Eisenstein series E_1.
 * https://oeis.org/A145154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, a(n)=4*A000005(n) where A000005(n) is the
 *            divisor-count function d(n)
 * Range:     u8 n <= 7559, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a simple incremental sieve to pre‑compute
   divisor counts up to the maximum argument received.  Results are cached
   for subsequent calls.  The algorithm runs in O(N log N) time where N
   is the largest n requested, which is easily fast enough for n≤10000. */

#include <stdlib.h>

/* Forward declaration – the calling environment provides `seq_t` and `idx_t`. */
extern seq_t a(idx_t n);

/* Internal static cache */
static seq_t *cache = NULL;          /* a(n) values */
static idx_t cache_size = 0;         /* number of entries in cache (counting from 0) */
static uint64_t *div_count = NULL;   /* divisor counts for 0..cache_size-1 */

/* Ensure that cache has entries up to and including n. */
static void ensure_cache(idx_t n) {
    /* No work if cache already covers n. */
    if (cache_size > n) return;

    /* Allocate space for new cache entries. */
    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    uint64_t *new_d = (uint64_t *)realloc(div_count, new_size * sizeof(uint64_t));
    if (!new_cache || !new_d) {
        /* Out of memory – abort; the contest infrastructure treats this
           as a failure. */
        abort();
    }
    cache = new_cache;
    div_count = new_d;

    /* Reset divisor counts and recompute for all numbers up to new_size-1. */
    for (size_t i = 0; i < new_size; ++i) {
        div_count[i] = 0;
    }
    for (size_t i = 1; i < new_size; ++i) {
        for (size_t j = i; j < new_size; j += i) {
            ++div_count[j];
        }
    }

    /* Fill the cache with the sequence values. */
    cache[0] = (seq_t)1;
    for (size_t i = 1; i < new_size; ++i) {
        cache[i] = (seq_t)(((seq_t)4) * (seq_t)div_count[i]);
    }

    cache_size = new_size;
}

seq_t a(idx_t n) {
    ensure_cache(n);
    return cache[n];
}
