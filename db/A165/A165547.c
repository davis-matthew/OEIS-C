/*
 * A165547  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * The sequence satisfies the linear recurrence derived from its rational
 * generating function:
 *   a(n) = 29*a(n-1)+29*a(n-2)+…+29*a(n-8)-435*a(n-9) + p(n)
 *
 * where the forcing sequence p is given by
 *   p(0)=1,   p(1)=2, …, p(8)=2,   p(9)=1,   p(n)=0 for n>9.
 * This follows from G(t)=(t^9+2t^8+…+2t+1)/(435t^9-29t^8-…-29t+1).
 * Initial values are a(0)=1, a(1)=31, a(2)=930, … (they are produced
 * automatically from the recurrence).
 */

#include <stdlib.h>

static const seq_t p[10] = {
    (seq_t)1ULL, /* p(0) */
    (seq_t)2ULL, /* p(1) */
    (seq_t)2ULL, /* p(2) */
    (seq_t)2ULL, /* p(3) */
    (seq_t)2ULL, /* p(4) */
    (seq_t)2ULL, /* p(5) */
    (seq_t)2ULL, /* p(6) */
    (seq_t)2ULL, /* p(7) */
    (seq_t)2ULL, /* p(8) */
    (seq_t)1ULL  /* p(9) */
};

/* Memoisation cache for computed terms */
static seq_t *cache     = NULL; /* array of a(n) values   */
static size_t cache_cap = 0;    /* allocated size of cache       */
static size_t cached_end = (size_t)-1; /* last index computed, 0=none */

/* Return a(n). */
seq_t a(idx_t n)
{
    /* Ensure cache has enough space. */
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        if (cache_cap < 10) cache_cap = 10;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failed */
    } else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 10) new_cap = 10;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n that are not yet known. */
    idx_t start = 0;
    if (cached_end != (size_t)-1) {
        if (n <= (idx_t)cached_end) {           /* already computed */
            return cache[(size_t)n];
        }
        start = (idx_t)cached_end + 1;
    }

    for (idx_t k = start; k <= n; ++k) {
        seq_t sum = (k < 10) ? p[(size_t)k] : (seq_t)0;

        /* Add 29 * a(k-j) for j = 1 .. 8  (if k >= j)   */
        for (idx_t j = 1; j <= 8 && k >= j; ++j) {
            sum += cache[(size_t)(k - j)] * (seq_t)29U;
        }

        /* Subtract 435 * a(k-9) when k >= 9   */
        if (k >= 9) {
            sum -= cache[(size_t)(k - 9)] * (seq_t)435U;
        }

        cache[(size_t)k] = sum;
    }

    cached_end = (size_t)n;
    return cache[(size_t)n];
}
