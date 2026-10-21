/*
 * A220683  Number of ways to reciprocally link elements of an n X 3 array
 *          either to themselves or to exactly two horizontal, vertical or
 *          antidiagonal neighbors.
 * https://oeis.org/A220683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 12*a(n-2) + 19*a(n-3) + 16*a(n-4) + 2*a(n-5)
 *            for n > 6. The first six terms are a(1)=1, a(2)=12, a(3)=94,
 *            a(4)=745, a(5)=5851, a(6)=46027
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache = NULL;            /* storage for computed values */
static size_t cache_size = 0;          /* number of elements allocated (index 0 unused) */
static idx_t  max_cached  = 0;         /* highest n for which cache[n] is ready */

/* Helper to resize the cache array.  Always sized to max(n+1, cache_size). */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)(n + 1); /* indices 1..n inclusive */
    if (needed <= cache_size)
        return;
    seq_t *new_cache = (seq_t*)realloc(cache, needed * sizeof(seq_t));
    if (!new_cache)
        return; /* silently fail; the caller will see zeroes */
    cache = new_cache;
    cache_size = needed;
}

/* Initialise the first six terms if they are not yet set. */
static void init_base_terms(void)
{
    if (max_cached >= 6)
        return;
    /* Only set terms that fit in current allocation. */
    if (cache_size > 1) cache[1] = (seq_t)1;
    if (cache_size > 2) cache[2] = (seq_t)12;
    if (cache_size > 3) cache[3] = (seq_t)94;
    if (cache_size > 4) cache[4] = (seq_t)745;
    if (cache_size > 5) cache[5] = (seq_t)5851;
    if (cache_size > 6) cache[6] = (seq_t)46027;
    max_cached = (cache_size > 6 ? 6 : (idx_t)(cache_size - 1));
}

/* Compute a(n) as requested, using memoisation. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for offset 1, but harmless */

    /* Make sure we have memory for index n. */
    ensure_capacity(n);

    /* Ensure base terms are set. */
    init_base_terms();

    /* Compute missing values up to n. */
    while (max_cached < n) {
        idx_t i = max_cached + 1;
        /* Use recurrence; all indices i-1 .. i-5 exist because i > 6. */
        seq_t val = (seq_t)6  * cache[i-1]
                  + (seq_t)12 * cache[i-2]
                  + (seq_t)19 * cache[i-3]
                  + (seq_t)16 * cache[i-4]
                  + (seq_t)2  * cache[i-5];
        cache[i] = val;
        max_cached = i;
    }

    return cache[n];
}
