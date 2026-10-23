/*
 * A022103  Fibonacci sequence beginning 1, 13.
 * https://oeis.org/A022103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) for n >= 2, * a(0) = 1, a(1) = 13
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* malloc, realloc */

static seq_t *cache      = NULL; /* cached values */
static idx_t  cache_size = 0;    /* allocated capacity */
static idx_t  next_comp  = 0;    /* first index not yet computed */

/* Return the n‑th term of A022103, computing it lazily and memoising */
seq_t a(idx_t n)
{
    /* Initialise cache on first call or when n is large enough */
    if (!cache) {
        /* Ensure space for at least two terms (a0 and a1). */
        cache_size = (n >= 1) ? (size_t)n + 1 : 2;
        cache      = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;                   /* out‑of‑memory fall‑back */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)13;
        next_comp = 2;                          /* next index to compute */
    }

    /* Grow cache if requested index exceeds current capacity */
    if ((size_t)n + 1 > cache_size) {
        size_t new_cap = cache_size ? cache_size * 2 : 2;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                     /* out‑of‑memory fall‑back */
        cache      = tmp;
        cache_size = new_cap;
    }

    /* Compute missing terms up to n */
    while (next_comp <= n) {
        cache[next_comp] = cache[next_comp - 1] + cache[next_comp - 2];
        ++next_comp;
    }

    return cache[n];
}
