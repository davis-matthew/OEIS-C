/*
 * A087432  Expansion of 1+x*(1-x-4*x^2)/((1+x)*(1-2*x)*(1-3*x)).
 * https://oeis.org/A087432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A087432
   Formula (n>0):  a(n) = (-1)^n/6 + 2^n/3 + 3^n/6
   Recurrence (n>4): a(n) = 6*a(n-1) - 9*a(n-2) - 4*a(n-3) + 12*a(n-4)
   Initial values: a(0)=1, a(1)=1, a(2)=3, a(3)=7, a(4)=19
*/

#include <stdlib.h>
#include <stdint.h>

/* Static cache to memoise computed terms.  All arithmetic is done in seq_t. */
static seq_t *cache = NULL;
static idx_t cache_size   = 0;   /* first index NOT yet computed */
static idx_t cache_alloc  = 0;   /* allocated capacity */

/* Ensure that cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;            /* already computed */

    /* Grow the array capacity if necessary. */
    idx_t new_cap = cache_alloc ? cache_alloc : 8;
    while (new_cap <= n) new_cap <<= 1;
    if (new_cap != cache_alloc) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) abort();                 /* out of memory */
        cache = tmp;
        cache_alloc = new_cap;
    }

    /* Initialise new slots to zero (for safety). */
    for (idx_t i = cache_size; i < cache_alloc; ++i) cache[i] = 0;

    /* Compute missing terms. */
    for (idx_t k = cache_size; k <= n; ++k) {
        if (k == 0)       cache[0] = 1;
        else if (k == 1)  cache[1] = 1;
        else if (k == 2)  cache[2] = 3;
        else if (k == 3)  cache[3] = 7;
        else if (k == 4)  cache[4] = 19;
        else
            cache[k] = 6 * cache[k-1] - 9 * cache[k-2] - 4 * cache[k-3] + 12 * cache[k-4];
    }
    cache_size = n + 1;
}

/* Sequence entry for index n. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
