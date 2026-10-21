/*
 * A099495  A Chebyshev transform of Fib(n)^2.
 * https://oeis.org/A099495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 30, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A099495: a(n) = 2*a(n-1) - a(n-2) + 3*a(n-3) - a(n-4) +
   2*a(n-5) - a(n-6);
   initial values: a(0)=0, a(1)=1, a(2)=1, a(3)=2, a(4)=6,
   a(5)=12, a(6)=25. */
#include <stdlib.h>

/* static cache for previously computed terms */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* ensure the cache contains terms up to n */
static void ensure_cache(idx_t n) {
    if (cache_len > n)
        return;                     /* already up to date */

    /* grow or allocate the cache */
    idx_t new_len = n + 1;
    seq_t *new_cache = NULL;
    if (cache)
        new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    else
        new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));

    if (!new_cache) {
        /* allocation failed – abort */
        exit(1);
    }

    cache = new_cache;

    /* initialise newly allocated entries */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)           cache[i] = 0;
        else if (i == 1)      cache[i] = 1;
        else if (i == 2)      cache[i] = 1;
        else if (i == 3)      cache[i] = 2;
        else if (i == 4)      cache[i] = 6;
        else if (i == 5)      cache[i] = 12;
        else if (i == 6)      cache[i] = 25;
        else {
            /* recurrence: a(n)=2a(n‑1)+3a(n‑3)+2a(n‑5) – (a(n‑2)+a(n‑4)+a(n‑6)) */
            seq_t t1 = 2 * cache[i - 1];
            t1 += 3 * cache[i - 3];
            t1 += 2 * cache[i - 5];
            seq_t t2 = cache[i - 2];
            t2 += cache[i - 4];
            t2 += cache[i - 6];
            cache[i] = t1 - t2;
        }
    }

    cache_len = new_len;
}

/* external linkage function required by the build */
seq_t a(idx_t n) {
    ensure_cache(n);
    return cache[n];
}
