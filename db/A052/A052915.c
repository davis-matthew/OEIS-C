/*
 * A052915  Expansion of (1-x)/(1 - x - x^2 - 3*x^3 + 3*x^4).
 * https://oeis.org/A052915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 3*a(n-3) - 3*a(n-4) Initial values:
 *            a(0)=1, a(1)=0, a(2)=1, a(3)=4 (Recurrence applies for n >= 4)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 35, u64 n <= 70, u128 n <= 138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of already computed terms */
static size_t cache_cap = 0;         /* allocated capacity (in elements) */
static size_t cache_len = 0;         /* number of valid elements in cache */

/* Ensure the cache contains all terms up to index n */
static void ensure_cache(idx_t n)
{
    /* If needed, grow the cache */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap <= (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!p) abort();          /* unrecoverable memory allocation failure */
        cache = p;
        cache_cap = new_cap;
    }

    /* Initialise the first four terms if the cache is empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;       /* a(0) */
        cache[1] = (seq_t)0;       /* a(1) */
        cache[2] = (seq_t)1;       /* a(2) */
        cache[3] = (seq_t)4;       /* a(3) */
        cache_len = 4;
    }

    /* Compute any missing terms up to n */
    if ((size_t)n + 1 > cache_len) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t t = cache[i-1]          /* a(i-1) */
                    + cache[i-2];        /* a(i-2) */
            t += cache[i-3] * (seq_t)3;   /* 3*a(i-3) */
            t -= cache[i-4] * (seq_t)3;   /* -3*a(i-4) */
            cache[i] = t;
        }
        cache_len = (size_t)n + 1;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
