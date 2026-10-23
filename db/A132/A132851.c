/*
 * A132851  a(0)=1. a(n) = the largest squarefree integer which divides
 *          (n+a(n-1)), for n>=1.
 * https://oeis.org/A132851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n >= 1, a(n) = largest squarefree divisor of (n +
 *            a(n-1))
 * Range:     u8 n <= 49, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   _radical
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a simple cache to memoise values and a small auxiliary routine that
   returns the product of distinct prime factors (the radical) of a seq_t. */

/* If seq_t is an arbitrary‑precision integer type, the routine still works
   because all arithmetic is performed by the type's overloaded operators
   (or its member functions). */

/* Compile‑time safety: no variable named 'new', matched C/C++ syntax. */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/_radical.h"

/* ---- Auxiliary ---------------------------------------------------- */

/* ---- Cache for memoisation -------------------------------------- */

static seq_t *cache      = NULL;   /* holds a(0), a(1), ... */
static size_t cache_cap  = 0;      /* allocated size of the array        */
static idx_t  cache_len  = 0;      /* number of entries that are valid   */

/* Ensure cache can hold at least indices 0 .. n. */
static void _ensure_capacity(idx_t n)
{
    if (cache_cap > (size_t)n)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                      /* allocation failure */

    /* Initialise newly allocated slots to zero (not strictly needed). */
    if (cache_cap < new_cap)
        for (size_t i = cache_cap; i < new_cap; ++i)
            new_cache[i] = (seq_t)0;

    cache      = new_cache;
    cache_cap  = new_cap;
}

/* ---- Main sequence function ------------------------------------- */

seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0)
        return (seq_t)1;

    /* Initialise cache on first call */
    if (cache_len == 0)
    {
        _ensure_capacity(0);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* If value already computed, return it */
    if (n < cache_len)
        return cache[n];

    /* Compute missing terms consecutively */
    _ensure_capacity(n);
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t sum = (seq_t)i + cache[i - 1];      /* n + a(n-1)  */
        cache[i]  = _radical(sum);                /* largest squarefree divisor */
    }

    cache_len = n + 1;
    return cache[n];
}
