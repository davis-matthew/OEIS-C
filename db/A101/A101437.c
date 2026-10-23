/*
 * A101437  a(1) = 1; a(n+1) = sum{k=1 to n} a(n+1-k)(mod k).
 * https://oeis.org/A101437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n+1) = Σ_{k=1..n} a(n+1-k) mod k for n ≥ 1
 * Range:     u8 n <= 42, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values: a[1] .. a[cache_max] */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* allocated array size (>= cache_max+1) */
static idx_t cache_max  = 0;   /* highest index whose value is valid */

/* Ensure that a[1..n] are computed and stored in `cache`. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max)               /* already available */
        return;

    /* Grow the cache to a power‑of‑two ≥ n+2 (one slot for index 0). */
    if (cache_size == 0)
        cache_size = 2;                /* at least two slots */
    while (cache_size < n + 2)
        cache_size <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
    cache = tmp;                       /* realloc may move the block */

    /* Initialize a(1) if the cache was empty. */
    if (cache_max == 0) {
        cache[1] = (seq_t)1;
        cache_max = 1;
    }

    /* Compute the new terms a[cache_max+1] .. a[n]. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k < i; ++k) {
            sum += cache[i - k] % (seq_t)k;
        }
        cache[i] = sum;
    }

    cache_max = n;
}

/* External‑linkage function required by the build. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;      /* undefined for n < 1, safe fallback */
    ensure_cache(n);
    return cache[n];
}
