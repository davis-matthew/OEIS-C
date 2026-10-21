/*
 * A068911  Number of n-step walks (each step +-1 starting from 0) which are
 *          never more than 2 or less than -2.
 * https://oeis.org/A068911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(0)=1, a(1)=2, a(2)=4; a(n)=3*a(n-2) for all n≥3
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 39, u64 n <= 80, u128 n <= 161,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This recurrence generates the OEIS sequence A068911: the number of
   n‑step walks from 0 with steps ±1 that never go below –2 or above 2. */

#include <stdlib.h>

static seq_t *cache = NULL;   /* dynamic array of computed terms */
static size_t   cache_len = 0; /* number of entries currently stored */
static size_t   cache_cap = 0; /* allocated size of the array */

seq_t a(idx_t n)
{
    /* Ensure the cache can hold terms up to index n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 1 : cache_cap;
        while (new_cap < (size_t)n + 1) new_cap *= 2;

        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();          /* out of memory */
        cache      = new_cache;
        cache_cap  = new_cap;
    }

    /* Compute any missing terms up to n inclusive */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i == 0)     cache[i] = (seq_t)1;
        else if (i == 1)cache[i] = (seq_t)2;
        else if (i == 2)cache[i] = (seq_t)4;
        else             cache[i] = cache[i-2] * (seq_t)3; /* recurrence */
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
