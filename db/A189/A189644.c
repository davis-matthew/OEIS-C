/*
 * A189644  Number of n X 3 array permutations with each element moving zero
 *          or one space horizontally, diagonally or antidiagonally.
 * https://oeis.org/A189644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 6*a(n-2) - 8*a(n-3) + 16*a(n-4) Initial
 *            values: a(1)=3, a(2)=33, a(3)=263, a(4)=2161
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;  /* stored values of the sequence   */
static size_t cache_cap = 0; /* current capacity of cache       */
static idx_t  cache_max = 0; /* number of values already known  */

/* Ensure the cache contains at least the first n terms. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max) return;   /* already have them               */

    /* Grow the cache if needed. */
    if (n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while ((idx_t)new_cap < n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* allocation failed                */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize the first four terms if this is the first call. */
    if (cache_max == 0) {
        if (cache_cap >= 1) cache[0] = (seq_t)3;
        if (cache_cap >= 2) cache[1] = (seq_t)33;
        if (cache_cap >= 3) cache[2] = (seq_t)263;
        if (cache_cap >= 4) cache[3] = (seq_t)2161;
        cache_max = 4;
    }

    /* Compute terms until we have the nth one. */
    while (cache_max < n) {
        idx_t i = cache_max + 1;                     /* 1‑based index of new term   */
        seq_t a1 = cache[i - 2];                    /* a(i-1)                      */
        seq_t a2 = cache[i - 3];                    /* a(i-2)                      */
        seq_t a3 = cache[i - 4];                    /* a(i-3)                      */
        seq_t a4 = cache[i - 5];                    /* a(i-4)                      */
        seq_t val = a1 * (seq_t)9
                    - a2 * (seq_t)6
                    - a3 * (seq_t)8
                    + a4 * (seq_t)16;
        cache[i - 1] = val;
        ++cache_max;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range, defined as 0 */
    ensure_cache(n);
    return cache[n - 1];           /* sequence is 1‑based          */
}
