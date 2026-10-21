/*
 * A204514  Numbers such that floor(a(n)^2 / 8) is again a square.
 * https://oeis.org/A204514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 27, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A204514: Numbers such that floor(a(n)^2/8) is again a square.
 * Recurrence (valid for n ≥ 6):
 *     a(1) = 0, a(2) = 1, a(3) = 2, a(4) = 3, a(5) = 6,
 *     a(n) = 6 * a(n-2) - a(n-4).
 *
 * This implementation memoises results in a static array and
 * grows the array as needed.
 */
#include <stdlib.h>

/* Cached values: cache[i] == a(i+1)  (cache[0] unused) */
static seq_t *cache = NULL;
static idx_t  cache_size = 0;

/* Ensure that the cache holds terms up to index 'n' (n ≥ 1). */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;           /* already computed */

    /* Allocate or grow the cache. */
    size_t new_sz = (size_t)n + 1;          /* +1 because we use 1‑based index */
    seq_t *new_cache;

    if (cache == NULL) {
        new_cache = (seq_t *)malloc(new_sz * sizeof(seq_t));
        if (!new_cache)          /* allocation failed – caller gets UB */
            return;
    } else {
        new_cache = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!new_cache)          /* allocation failed – caller gets UB */
            return;
    }

    /* Set the known starting values. */
    if (new_sz >= 2) new_cache[1] = 0;          /* a(1) */
    if (new_sz >= 3) new_cache[2] = 1;          /* a(2) */
    if (new_sz >= 4) new_cache[3] = 2;          /* a(3) */
    if (new_sz >= 5) new_cache[4] = 3;          /* a(4) */
    if (new_sz >= 6) new_cache[5] = 6;          /* a(5) */

    /* Compute missing terms using the recurrence. */
    for (idx_t i = 6; i <= n; ++i) {
        seq_t t = new_cache[i-2];
        t = t * 6;            /* 6 * a(i-2) */
        t = t - new_cache[i-4];
        new_cache[i] = t;
    }

    cache = new_cache;
    cache_size = new_sz;
}

/* Return the nth term of the sequence (n ≥ 1).  For n = 0 the function
 * returns 0, although the sequence is defined only for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* safety for callers using 0‑based indices */
    ensure_cache(n);
    return cache[n];
}
