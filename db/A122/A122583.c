/*
 * A122583  a(n) = a(n - 1) - 2*a(n - 2) + a(n - 3) - 6*a(n - 4) + 3*a(n - 5).
 * https://oeis.org/A122583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - 2*a(n-2) + a(n-3) - 6*a(n-4) + 3*a(n-5) with
 *            a(1)=a(2)=a(3)=a(4)=a(5)=1 (offset 1)
 * Range:     i8 n <= 13, i16 n <= 23, i32 n <= 47, i64 n <= 89, i128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static storage for memoisation */
static seq_t *cache = NULL;   /* array indexed 0..n (0 unused) */
static size_t   cap   = 0;    /* allocated capacity (indices 0..cap-1) */
static idx_t    max_cached = 0; /* highest n that has been computed */

/* ensure capacity for indices 0..n inclusive */
static void ensure_cache_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cap < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();            /* out‑of‑memory: terminate */
        cache = tmp;
        cap   = needed;
    }
}

/* initialise first five terms (called once) */
static void init_cache(void)
{
    ensure_cache_capacity(5);
    cache[1] = cache[2] = cache[3] = cache[4] = cache[5] = (seq_t)1;
    max_cached = 5;
}

/* The required sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1 */

    /* first call: initialise */
    if (!max_cached) init_cache();

    /* compute missing terms up to n */
    if (n > max_cached) {
        ensure_cache_capacity(n);
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            seq_t v = cache[i-1];
            v -= cache[i-2] * (seq_t)2;
            v += cache[i-3];
            v -= cache[i-4] * (seq_t)6;
            v += cache[i-5] * (seq_t)3;
            cache[i] = v;
        }
        max_cached = n;
    }

    return cache[n];
}
