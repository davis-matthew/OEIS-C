/*
 * A008896  3x - 1 sequence starting at 66.
 * https://oeis.org/A008896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A008896: 3x-1 sequence starting at 66
   Recurrence: a(0) = 66,
               a(n) = a(n-1)/2  if a(n-1) even,
                       3*a(n-1)-1 otherwise */
#include <stdlib.h>
#include <stddef.h>

/* Ensure that the cache can hold at least 'needed' values. */
static void ensure_capacity(size_t *cap, size_t needed, seq_t **arr)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap > 0) ? (*cap) * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    *arr = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* Static cache for memoised values. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;   /* number of entries currently stored */
static size_t cache_cap  = 0;   /* current allocated capacity */

/* Compute a(n). */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;      /* n will be <= 10000 in tests */

    /* Return cached value if already computed. */
    if (idx < cache_len) return cache[idx];

    /* First call: allocate initial cache. */
    if (!cache) {
        cache_cap = 8;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    /* Make sure we can store up to idx. */
    ensure_capacity(&cache_cap, idx + 1, &cache);

    /* Initialise the very first term. */
    if (cache_len == 0) {
        cache[0] = (seq_t)66;
        cache_len = 1;
    }

    /* Compute missing terms up to idx. */
    for (size_t i = cache_len; i <= idx; ++i) {
        seq_t prev = cache[i - 1];
        cache[i] = (prev & (seq_t)1) == 0  /* even? */
                   ? prev / (seq_t)2
                   : prev * (seq_t)3 - (seq_t)1;
    }
    cache_len = idx + 1;
    return cache[idx];
}
