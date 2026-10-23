/*
 * A029760  A sum with next-to-central binomial coefficients of even order,
 *          Catalan related.
 * https://oeis.org/A029760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n+1) = 4*a(n) + binomial(2(n+2), n+1) (OEIS A029760)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   binom_2kplus4_kplus1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */
#include <stddef.h>   /* size_t */
#include "../utils/binom_2kplus4_kplus1.h"

/*--------------------------------- cache handling ---------------------------------*/
static seq_t *cache = NULL;     /* array holding computed a(n) values */
static size_t cache_cap = 0;    /* current capacity of the array          */
static idx_t  last_filled = 0;  /* largest index that has been computed   */

/* Grow the cache so that it can hold entries up to index n.  New slots are
   initialized to zero.  The cast of realloc is required for arbitrary‑precision
   builds where seq_t is a class type. */
static void grow_cache(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;

    size_t old_cap = cache_cap;
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;

    for (size_t i = old_cap; i < new_cap; ++i)
        cache[i] = 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Ensure the cache can hold entries up to n */
    grow_cache(n);

    /* initialise a(0) if it hasn't been computed yet */
    if (cache[0] == 0)
    {
        cache[0] = 1;
        last_filled = 0;
    }

    /* If we already have computed up to n, just return the value */
    if (last_filled >= n)
        return cache[n];

    /* Compute missing values up to n using the recurrence */
    for (idx_t i = last_filled; i < n; ++i) {
        seq_t bin = binom_2kplus4_kplus1(i);          /* C(2i+4, i+1) */
        cache[i + 1] = 4 * cache[i] + bin;
    }
    last_filled = n;                 /* largest index now computed */
    return cache[n];
}
