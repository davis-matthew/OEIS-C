/*
 * A052463  a(n) is the smallest nonnegative solution k to 24*k == 1 (mod
 *          7^(2*n-2)).
 * https://oeis.org/A052463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 49*a(n-1) - 2 for n > 2, with a(1) = 0, a(2) = 47. The
 *            recurrence comes directly from the OEIS entry and generates the
 *            sequence exactly as required. No modular arithmetic or large‑mod
 *            inverses are needed
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, free */

static seq_t *cache = NULL;      /* 1‑based array of previously computed values */
static size_t cache_size = 0;    /* number of allocated elements (including index 0) */

/* External linkage as requested */
seq_t a(idx_t n)
{
    /* Handle the two defined base cases immediately */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)47;

    /* Allocate initial cache if this is the first call */
    if (!cache) {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));  /* indices 0,1,2 used; 0 unused */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)47;
        cache_size = 3;
    }

    /* Remember the size before (possible) reallocation */
    size_t old_size = cache_size;

    /* Grow the cache to accommodate index n if necessary */
    if (n + 1 > cache_size) {
        seq_t *new_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        for (size_t i = 1; i < cache_size; ++i)
            new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_size = n + 1;
    }

    /* Compute any missing terms up to n using the recurrence */
    for (size_t i = old_size; i <= n; ++i) {
        if (i == 1)       cache[i] = (seq_t)0;
        else if (i == 2)  cache[i] = (seq_t)47;
        else              cache[i] = cache[i - 1] * (seq_t)49 - (seq_t)2;
    }

    return cache[n];
}
