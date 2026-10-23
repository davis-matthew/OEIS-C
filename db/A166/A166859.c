/*
 * A166859  Number of reduced words of length n in Coxeter group on 5
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^13 = I.
 * https://oeis.org/A166859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:   a(0)=1, a(1)=5, … , a(12)=20971520.
   For n >= 14 : a(n) = 3·(a(n‑1)+…+a(n‑12)) – 6·a(n‑13).
   For n = 13 : a(13)=3·(a(12)+…+a(1)) – 6·a(0)+1. */

#include <stdlib.h>

static const seq_t init[13] = {
    (seq_t)1,      (seq_t)5,      (seq_t)20,     (seq_t)80,
    (seq_t)320,    (seq_t)1280,   (seq_t)5120,   (seq_t)20480,
    (seq_t)81920,  (seq_t)327680, (seq_t)1310720, (seq_t)5242880,
    (seq_t)20971520
};

static seq_t *cache = NULL;        /* cache of computed terms */
static idx_t cache_cap = 0;        /* size of allocated array */
static idx_t last_computed = (idx_t)-1; /* largest index that has a value */

/* Grow the cache to at least new_cap entries. */
static void grow_cache(idx_t new_cap)
{
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Return the n‑th term of sequence A166859. */
seq_t a(idx_t n)
{
    if (n < 13)                      /* initial segment */
        return init[n];

    /* Initialise cache on first use. */
    if (cache == NULL) {
        idx_t cap = (n + 1 > 13) ? (n + 1) : 13;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache_cap = cap;
        for (idx_t i = 0; i < 13; ++i)
            cache[i] = init[i];
        for (idx_t i = 13; i < cap; ++i)
            cache[i] = 0;
        last_computed = 12;
    }

    if (n + 1 > cache_cap)
        grow_cache(n + 1);

    if (n <= last_computed)          /* already computed */
        return cache[n];

    /* Compute missing terms up to n. */
    for (idx_t k = last_computed + 1; k <= n; ++k) {
        seq_t sum12 = 0;
        for (idx_t j = 1; j <= 12; ++j)
            sum12 += cache[k - j];
        if (k == 13)
            cache[13] = sum12 * (seq_t)3 - cache[0] * (seq_t)6 + (seq_t)1;
        else
            cache[k] = sum12 * (seq_t)3 - cache[k - 13] * (seq_t)6;
    }

    last_computed = n;
    return cache[n];
}
