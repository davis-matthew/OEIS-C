/*
 * A183330  Number of n X 2 binary arrays with each 1 adjacent to exactly two
 *          0's.
 * https://oeis.org/A183330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + a(n-3) - a(n-4); initial values:
 *            a(1)=1, a(2)=7, a(3)=10, a(4)=23
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free, abort */

/* static cache for computed values */
static seq_t *cache = NULL;          /* 1‑based index: cache[i] is a(i) */
static size_t cached = 0;            /* largest n computed so far */
static size_t cache_cap = 0;         /* current allocated capacity (≥ cached) */

/* make sure cache contains values up to at least n (n ≥ 1) */
static void ensure_cache(idx_t n)
{
    if ((size_t)n <= cached) return;        /* already computed */

    /* ensure we have space for indices 1 … n (plus one for unused 0) */
    size_t new_cap = (size_t)n + 1;
    if (new_cap < 5) new_cap = 5;           /* need room for the 4 seeds */

    if (new_cap > cache_cap) {
        seq_t *tmp = NULL;
        if (cache == NULL) {
            tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        if (!tmp) abort();                 /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the four seed values on the first allocation */
    if (cached == 0) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)10;
        cache[4] = (seq_t)23;
        cached = 4;
    }

    /* compute missing terms up to n */
    for (size_t i = cached + 1; i <= (size_t)n; ++i) {
        cache[i] = cache[i-1] + (seq_t)2 * cache[i-2]
                     + cache[i-3] - cache[i-4];
    }
    if (cached < (size_t)n) cached = (size_t)n;
}

/* Number of n × 2 binary arrays with each 1 adjacent to exactly two 0's */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* undefined for n < 1 */
    ensure_cache(n);
    return cache[(size_t)n];
}
