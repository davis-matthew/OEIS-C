/*
 * A076630  a(n) is the smallest number such that product of first n terms is
 *          strictly greater than n-th power of a(n-1) starting with a(1)=1.
 * https://oeis.org/A076630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   power_simple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A076630
 *
 *  Formula: a(1) = 1
 *           a(n) = 1 + ⌊ a(n‑1)^n / ∏_{i=1}^{n‑1} a(i) ⌋   for n > 1
 *
 *  The implementation keeps a dynamic cache of the terms and their
 *  cumulative product.  A small iterative power routine is used to
 *  avoid a missing _pow_seq_t helper.  The code works for the builtin
 *  integer types and for the arbitrary‑precision seq_t that the build
 *  supplies.
 */
#include <stdlib.h>   /* malloc, realloc */
#include "../utils/power_simple.h"

/* Static cache of computed terms (1‑based index) */
static seq_t *cache     = NULL;  /* cache[i] == a(i+1) */
static idx_t  cache_len = 0;     /* number of terms stored (a(1) ... a(cache_len)) */
static seq_t  prod      = 0;     /* product a(1)…a(cache_len) */

/* Initialise the cache with a(1)=1.  Called lazily on first request. */
static void ensure_cache(void)
{
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = (seq_t)1;   /* a(1) */
        prod    = (seq_t)1;    /* product so far */
        cache_len = 1;
    }
}

/* Return the n-th term of A076630 (n >= 1).  Calls may arrive in any order. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, return 0 as a guard */

    ensure_cache();

    /* Compute up to the requested index */
    while (cache_len < n) {
        idx_t   k   = cache_len + 1;                 /* next index (1‑based) */
        seq_t   prev = cache[cache_len - 1];         /* a(k-1) */

        /* numerator = a(k-1)^k */
        seq_t numer = power_simple(prev, k);

        /* ratio = floor(numer / prod) */
        seq_t ratio = numer / prod;

        /* a(k) = 1 + ratio */
        seq_t ak = ratio + (seq_t)1;

        /* grow the cache */
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)(cache_len + 1) * sizeof(seq_t));
        cache = tmp;   /* realloc will not return NULL for the small sizes needed */
        cache[cache_len] = ak;

        /* update the product for the next iteration */
        prod *= ak;
        cache_len++;
    }

    return cache[n - 1];
}
