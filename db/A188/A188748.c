/*
 * A188748  Number of 3 X n binary arrays without the pattern 1 1 1
 *          diagonally, vertically or antidiagonally.
 * https://oeis.org/A188748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 2*a(n-2) + 11*a(n-3) + 10*a(n-4) - 30*a(n-5) -
 *            12*a(n-6) (valid for n >= 7) with initial values: a(1)=7,
 *            a(2)=49, a(3)=292, a(4)=1723, a(5)=10327, a(6)=61996
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_values[7] = { 0,
    7ULL,    /* a(1) */
    49ULL,   /* a(2) */
    292ULL,  /* a(3) */
    1723ULL, /* a(4) */
    10327ULL,/* a(5) */
    61996ULL /* a(6) */
};

static seq_t *cache = NULL;        /* array of computed terms, index 1..n */
static idx_t cache_cap = 0;        /* allocated capacity (size-1) */
static idx_t cache_last = 0;       /* highest index already computed */

/* Grow cache to at least size n+1 (indexing from 0). */
static void ensure_capacity(idx_t n)
{
    if (n <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < n)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
    /* Newly allocated entries are zeroed by realloc, which is okay because
       we only read written positions. */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;    /* unneeded, but safe for non‑positive indices */

    /* Handle the first six values directly. */
    if (n <= 6)
        return init_values[n];

    /* Ensure the cache can hold index n. */
    ensure_capacity(n);

    /* If this is the first call, initialise the cache with the first six terms. */
    if (cache_last == 0) {
        cache[1] = init_values[1];
        cache[2] = init_values[2];
        cache[3] = init_values[3];
        cache[4] = init_values[4];
        cache[5] = init_values[5];
        cache[6] = init_values[6];
        cache_last = 6;
    }

    /* If the requested term is already cached, simply return it. */
    if (n <= cache_last)
        return cache[n];

    /* Compute terms from cache_last+1 up to n using the recurrence. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        cache[i] = 6 * cache[i - 1] - 2 * cache[i - 2]
                 + 11 * cache[i - 3] + 10 * cache[i - 4]
                 - 30 * cache[i - 5] - 12 * cache[i - 6];
    }
    cache_last = n;
    return cache[n];
}
