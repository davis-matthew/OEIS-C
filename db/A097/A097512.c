/*
 * A097512  a(n) = 6*Lucas(2n) - Fibonacci(2n+2).
 * https://oeis.org/A097512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2), a(0)=11, a(1)=15
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A097512 efficiently and memoises results.
 * The recurrence guarantees O(1) amortised time per term and
 * holds for all n ≥ 0. The algorithm uses a static cache that is
 * lazily grown to the requested index.  All arithmetic is performed
 * on the seq_t type so that it works correctly for 8,16,32,64,128‑bit
 * integers and arbitrary‑precision integer builds.                                                           */
#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* dynamic array of computed terms      */
static size_t cache_size = 0;     /* number of valid entries              */
static size_t cache_cap  = 0;     /* current allocated capacity           */

/* Ensure the cache is valid up to the supplied index `n`. */
static void
ensure_cache(idx_t n)
{
    /* enlarge capacity if necessary */
    if (cache_cap <= (size_t)n)
    {
        size_t new_cap = (cache_cap == 0) ? 4 : cache_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* out of memory */
        seq_cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise base cases if needed */
    if (cache_size == 0)
    {
        seq_cache[0] = (seq_t)11;
        cache_size = 1;
    }
    if (cache_size == 1 && n >= 1)
    {
        seq_cache[1] = (seq_t)15;
        cache_size = 2;
    }

    /* compute further terms up to `n` */
    for (size_t i = cache_size; i <= (size_t)n; ++i)
    {
        seq_cache[i] = (seq_t)3 * seq_cache[i - 1] - seq_cache[i - 2];
    }
    cache_size = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return seq_cache[n];
}
