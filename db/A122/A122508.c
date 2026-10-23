/*
 * A122508  G.f.: 1/[(1-2x)(1+2x+3x^2)].
 * https://oeis.org/A122508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 6*a(n-3) with initial values a(0)=1, a(1)=0,
 *            a(2)=1. (This follows from 1/(1 - x^2 - 6x^3).)
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;          /* array of computed terms */
static size_t cache_len = 0;         /* number of terms that have been computed (0..len-1) */
static size_t cache_cap = 0;         /* allocated capacity of cache */

/* Ensure that cache holds values up to idx (inclusive). */
static void ensure_cache(idx_t idx)
{
    if (cache_len > (size_t)idx)
        return;   /* already computed */

    /* Determine required capacity, doubling until > idx. */
    size_t new_cap = cache_cap ? cache_cap : 3;
    while (new_cap <= (size_t)idx)
        new_cap <<= 1;

    /* Reallocate (may grow).  Cast needed for C++. */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();
    cache = new_cache;
    cache_cap = new_cap;

    /* Initialize the first three terms (safe even if they were already set). */
    cache[0] = (seq_t)1;
    if (new_cap > 1)
        cache[1] = (seq_t)0;
    if (new_cap > 2)
        cache[2] = (seq_t)1;

    /* Compute missing terms. */
    idx_t start = cache_len;
    if (start < 3)
        start = 3;
    for (idx_t i = start; i <= idx; ++i)
        cache[i] = cache[i - 2] + ((seq_t)6 * cache[i - 3]);

    cache_len = (size_t)idx + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
