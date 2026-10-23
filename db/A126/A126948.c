/*
 * A126948  The sequence d[n] defined in A126940.
 * https://oeis.org/A126948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=0, a(2)=4, a(3)=12, a(4)=34, a(5)=110;
   a(n) = 2*a(n-1) + a(n-2) + 7*a(n-3) + 4*a(n-4) + 4*a(n-5) + a(n-6)  (n ≥ 6)
   (derived from the rational generating function of A126948). */

#include <stdlib.h>

static seq_t *cache = NULL;          /* stored values, starting at index 0   */
static idx_t cache_size = 0;         /* allocated size of 'cache' (in terms) */
static idx_t computed_size = 0;      /* number of terms already computed */

/* Ensure the cache can hold indices up to n (inclusive). */
static void
ensure_cache(idx_t n)
{
    size_t needed = (size_t) (n + 1);
    if (cache_size >= needed)
        return;
    /* Allocate new larger buffer and copy old values. */
    seq_t *new_cache = (seq_t *)malloc(needed * sizeof(seq_t));
    if (new_cache == NULL)
        exit(1); /* out‑of‑memory */
    for (size_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];
    /* Initialize new portion to zero (will be overwritten). */
    for (size_t i = cache_size; i < needed; ++i)
        new_cache[i] = 0;
    free(cache);
    cache = new_cache;
    cache_size = (idx_t)needed;
}

/* Compute a(n) using the recurrence and memoisation. */
seq_t a(idx_t n)
{
    /* Grow the cache if necessary. */
    ensure_cache(n);

    /* Initialise the first six terms if they are not yet computed.   */
    if (computed_size < 6) {
        /* Set known starting values for 0 … min(5, cache_size-1) */
        if (cache_size > 0) cache[0] = 0;
        if (cache_size > 1) cache[1] = 0;
        if (cache_size > 2) cache[2] = 4;
        if (cache_size > 3) cache[3] = 12;
        if (cache_size > 4) cache[4] = 34;
        if (cache_size > 5) cache[5] = 110;
        if (computed_size < 6)
            computed_size = (cache_size < 6) ? cache_size : 6;
    }

    /* Compute missing terms up to n using the recurrence. */
    if (n >= 6 && computed_size <= n) {
        for (idx_t i = computed_size; i <= n; ++i) {
            seq_t val = 0;
            val += 2 * cache[i - 1];
            val += cache[i - 2];
            val += 7 * cache[i - 3];
            val += 4 * cache[i - 4];
            val += 4 * cache[i - 5];
            val += cache[i - 6];
            cache[i] = val;
        }
        computed_size = n + 1;
    }

    return cache[n];
}
