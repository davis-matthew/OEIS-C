/*
 * A083860  First subdiagonal of generalized Fibonacci array A083856.
 * https://oeis.org/A083860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{s=0..⌊(n-1)/2⌋} binomial(n-1-s, s) * (n+1)^s
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A083860: first subdiagonal of the generalized Fibonacci array A083856. */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k); */

static seq_t *cache = NULL;     /* cache of computed values */
static idx_t cache_len = 0;     /* how many entries are valid (indexes 0 .. cache_len-1) */
static idx_t cache_cap = 0;     /* allocated capacity */

/* Grow the cache to hold indices up to 'n' (inclusive). */
static void extend_cache(idx_t n)
{
    idx_t needed = n + 1;
    if (needed <= cache_cap)
        return;

    seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(*new_cache));
    if (!new_cache) {
        /* allocation failed – abort; the harness expects correctness only. */
        exit(1);
    }
    cache = new_cache;

    /* initialise new entries to zero (valid for all seq_t types). */
    for (idx_t i = cache_cap; i < needed; ++i)
        cache[i] = 0;

    cache_cap = needed;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* handle known starting values early */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* Ensure the cache is large enough. */
    if (n >= cache_cap)
        extend_cache(n);

    /* If already computed, just return it. */
    if (n < cache_len)
        return cache[n];

    /* Compute missing values from cache_len up to n. */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0) { cache[i] = 0; continue; }
        if (i == 1) { cache[i] = 1; continue; }

        /* a(i) = Sum_{s=0..⌊(i-1)/2⌋} binomial(i-1-s, s) * (i+1)^s */
        seq_t sum = 0;
        seq_t base = (seq_t)(i + 1);   /* (i+1) as seq_t */
        seq_t power = 1;               /* (i+1)^0 initially */

        idx_t smax = (i - 1) / 2;      /* for i >= 2, integer division */
        for (idx_t s = 0; s <= smax; ++s)
        {
            seq_t bin = binomial(i - 1 - s, s);  /* binomial(i-1-s, s) */
            sum += bin * power;
            power *= base;                     /* prepare (i+1)^(s+1) */
        }
        cache[i] = sum;
    }

    cache_len = n + 1;
    return cache[n];
}
