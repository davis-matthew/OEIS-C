/*
 * A179556  a(1) = 10; a(n) = a(n-1)*10 - 2^(n-2).
 * https://oeis.org/A179556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A179556:  a(1) = 10;  
    a(n) = a(n-1)*10 - 2^(n-2)  for n > 1.  (OEIS offset 1) */

#include <stdlib.h>

/* Note: cache is 1‑indexed; cache[0] is unused. */
static seq_t *cache = NULL;          /* buffer for computed terms */
static idx_t cache_size = 0;         /* highest index written so far */
static seq_t pow2 = (seq_t)1;        /* 2^(cache_size-1), ready for next term */

/* Compute possibly non‑sequential a(n).  The cache is extended when needed. */
seq_t a(idx_t n)
{
    /* Ensure the cache has space for n+1 entries (index 0 unused). */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        if (cache == NULL) return 0;  /* allocation failed – unlikely */
        cache_size = 0;
    }

    /* Initialise the first term if it hasn't been done yet. */
    if (cache_size == 0) {
        cache[1] = (seq_t)10;
        cache_size = 1;
        pow2 = (seq_t)1;   /* 2^(1-2) = 1, ready for a(2) */
    }

    /* Extend the cache up to n, computing missing terms. */
    if (n > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (tmp == NULL) return 0;  /* realloc failed */
        cache = tmp;

        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t next = prev * (seq_t)10 - pow2;
            cache[i] = next;
            pow2 <<= 1;           /* advance to 2^(i-1) for the next iteration */
        }
        cache_size = n;
    }

    return cache[n];
}
