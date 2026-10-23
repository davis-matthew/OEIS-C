/*
 * A126604  a(1) = 4; a(2) = 3; for n > 2, a(n) = a(n-1)^2 + a(n-1) - 1.
 * https://oeis.org/A126604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)^2 + a(n-1) - 1 (n > 2) Initial values: a(1) = 4,
 *            a(2) = 3
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* 1‑based array: cache[i] = a(i) */
static idx_t  cache_max = 0;  /* highest index currently cached */

/* Compute sequence term a(n) */
seq_t a(idx_t n)
{
    /* Handle the two base cases immediately */
    if (n == 1) return 4;
    if (n == 2) return 3;

    /* Ensure the cache is allocated and contains the base values */
    if (cache == NULL) {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));  /* indices 0,1,2 */
        if (!cache) return 0;                       /* allocation failed */
        cache[0] = 0;          /* unused */
        cache[1] = 4;          /* a(1) */
        cache[2] = 3;          /* a(2) */
        cache_max = 2;
    }

    /* Expand the cache if necessary */
    if (cache_max < n) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;    /* reallocation failed */
        cache = tmp;
        /* initialize unused entries to 0 for safety */
        for (idx_t i = cache_max + 1; i <= n; ++i)
            cache[i] = 0;
    }

    /* Compute new terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        cache[i] = prev * prev + prev - 1;
    }
    cache_max = n;

    return cache[n];
}
