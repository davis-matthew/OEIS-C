/*
 * A204200  INVERT transform of [1, 0, 1, 3, 9, 27, 81, ...].
 * https://oeis.org/A204200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) + a(n-3) initial values: a(1)=1,
 *            a(2)=1, a(3)=2 (Recurrence applies for n>=4)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 41, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;         /* 1‑based indexing: cache[1]..cache[n] */
static idx_t  cache_size = 0;       /* highest index currently stored */

/* Ensure that cache contains values up to index n. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;          /* nothing to do */

    /* Allocate space for at least indices 1…3, plus up to n. */
    idx_t new_size = n + 1;               /* array size (including index 0 unused) */
    if (new_size < 4) new_size = 4;       /* need room for the first three terms */

    /* Allocate / grow the cache.  Cast required for C++ compatibility. */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);  /* memory exhausted */

    /* On the first allocation, initialise the base values. */
    if (cache_size == 0) {
        new_cache[1] = (seq_t)1;
        new_cache[2] = (seq_t)1;
        new_cache[3] = (seq_t)2;
        cache_size = 3;                   /* we now know up to a(3) */
    }

    /* Compute new terms beyond what we already have. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t term =
            (seq_t)4 * new_cache[i-1] -
            (seq_t)3 * new_cache[i-2] +
            new_cache[i-3];
        new_cache[i] = term;
    }

    cache = new_cache;        /* update the global pointer after successful realloc */
    if (n > cache_size) cache_size = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for indices below the offset */
    ensure_cache(n);
    return cache[n];
}
