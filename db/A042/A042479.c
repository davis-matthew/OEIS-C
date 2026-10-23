/*
 * A042479  Denominators of continued fraction convergents to sqrt(767).
 * https://oeis.org/A042479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n-th convergent to √767. It satisfies
 *            a(0) = 1, a(1) = 1, a(2) = 3, a(3) = 10, … Recurrence: a(n) =
 *            a(n-1) * cf[(n-1) mod 12] + a(n-2)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 25, u64 n <= 49, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic part of the continued fraction for √767. */
#include <stdlib.h>

static const seq_t cf[12] = {1, 2, 3, 1, 1, 1, 1, 1, 3, 2, 1, 54};

static seq_t *cache = NULL;      /* memoised values of a(n)          */
static idx_t cache_cap = 0;      /* current size of cache array      */

/* Grow the cache to contain at least n+1 elements.           */
static void ensure_capacity(idx_t n) {
    if (cache_cap > n) return;
    idx_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= n) new_cap <<= 1;      /* exponential growth            */
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!cache) exit(1);                     /* out‑of‑memory check           */
    cache_cap = new_cap;
}

/* Value of the sequence for any n >= 0.                           */
seq_t a(idx_t n) {
    /* Initialise the cache on first use. */
    if (cache_cap == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) exit(1);
        cache_cap = 1;
        cache[0] = 1;                        /* a(0) */
    }

    /* If the requested index is already cached, return it.      */
    if (n < cache_cap) return cache[n];

    /* Compute any missing terms up to n, enlarging the cache as needed. */
    idx_t start = cache_cap;                 /* first term to compute       */
    ensure_capacity(n);                      /* guarantees space up to n   */
    for (idx_t i = start; i <= n; ++i) {
        seq_t prev      = cache[i-1];
        seq_t prevprev  = (i >= 2) ? cache[i-2] : (seq_t)0;
        seq_t coeff     = cf[(size_t)((i-1) % 12)];
        cache[i] = coeff * prev + prevprev;
    }
    cache_cap = n + 1;
    return cache[n];
}
