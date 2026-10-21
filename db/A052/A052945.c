/*
 * A052945  Number of compositions of n when each odd part can be of two
 *          kinds.
 * https://oeis.org/A052945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1 a(1) = 2 a(2) = 5 a(n) = 2 * ( a(n-1) + a(n-2) ) for n
 *            >= 3 (Recurrence deduced from the generating function )
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;   /* sequence table */
static size_t cache_cap  = 0;       /* allocated capacity */
static size_t cache_len  = 0;       /* number of stored terms */

/* Ensure that terms up to index n are computed and stored in cache[]. */
static void ensure_terms(idx_t n)
{
    /* Convert the unsigned index to size_t safely for array indices. */
    size_t idx = (size_t)n;

    /* Initialize cache if empty. */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t*)realloc(NULL, cache_cap * sizeof(seq_t));
        /* first three terms (index 0,1,2) */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)5;
        cache_len = 3;
    }

    /* Resize if necessary. */
    if (idx >= cache_cap) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 4;
        while (idx >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    if (idx >= cache_len) {
        for (size_t i = cache_len; i <= idx; ++i) {
            seq_t sum = cache[i-1] + cache[i-2];
            cache[i] = sum + sum;          /* 2 * sum */
        }
        cache_len = idx + 1;
    }
}

seq_t a(idx_t n)
{
    /* Base cases without touching the cache. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;

    ensure_terms(n);
    return cache[(size_t)n];
}
