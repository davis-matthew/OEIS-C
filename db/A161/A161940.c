/*
 * A161940  a(n) = ((3+sqrt(2))*(5+sqrt(2))^n + (3-sqrt(2))*(5-sqrt(2))^n)/2.
 * https://oeis.org/A161940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A161940
   Formula: a(0) = 3, a(1) = 17, a(n) = 10*a(n-1) - 23*a(n-2) for n > 1
   (computed via the recurrence, with O(1) memory for the current two
    terms and memoisation for random‑order access) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the two initial values directly. */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)17;

    /* Lazy allocation of the cache. */
    static seq_t *cache = NULL;   /* array of computed terms       */
    static size_t cache_len = 0;  /* number of terms stored so far */
    static size_t cache_cap = 0;  /* current allocation capacity   */

    /* Initialise the cache if it does not exist yet. */
    if (cache == NULL) {
        cache_cap = 2;
        cache_len = 2;
        /* In C++ malloc returns void*, cast to the proper pointer type. */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* out‑of‑memory: safe fallback   */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)17;
    }

    /* Grow the allocation if necessary. */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* out‑of‑memory: safe fallback   */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* a(i) = 10*a(i-1) - 23*a(i-2) */
        cache[i] = cache[i - 1] * (seq_t)10 - cache[i - 2] * (seq_t)23;
    }
    cache_len = (size_t)n + 1;  /* we now have terms 0..n stored */

    return cache[(size_t)n];
}
