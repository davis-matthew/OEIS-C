/*
 * A122169  a(1)=1. a(n) = sum of earlier terms, a(k) (1<=k<=n-1), where
 *          gcd(a(k),n) is squarefree.
 * https://oeis.org/A122169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, for n>1 a(n)= Σ_{k=1}^{n-1} a(k) if gcd(a(k),n) is
 *            squarefree
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 42, u64 n <= 85, u128 n <= 170,
 *            big any n
 * Helpers:   gcd, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External functions provided by the build environment */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"           /* seq_t gcd(seq_t a, seq_t b); */
#include "../utils/is_squarefree.h" /* bool is_squarefree(seq_t x); */

static seq_t *cache = NULL;     /* 1‑indexed array of computed a(n) values */
static size_t cache_cap = 0;    /* allocated capacity of the cache */
static idx_t cached_up_to = 0;  /* highest n for which a(n) is stored */

/* Ensure the cache can hold values up to n (inclusive). */
static void grow_cache(idx_t n)
{
    if (cache_cap <= (size_t)n)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                      /* undefined for n<1 */

    if (n == 1) return 1;

    /* Allocate cache and set base case if it hasn't been done yet */
    if (cache_cap == 0)
        grow_cache(1);                      /* at least space for a[1] */
    if (cached_up_to == 0)                 /* first time */
    {
        cache[1] = 1;
        cached_up_to = 1;
    }

    /* Compute all terms up to n that haven't been computed yet */
    if (cached_up_to < n)
    {
        grow_cache(n);                      /* ensure room up to n */

        for (idx_t i = cached_up_to + 1; i <= n; ++i)
        {
            seq_t sum = 0;
            seq_t idx_val = (seq_t)i;
            for (idx_t k = 1; k < i; ++k)
            {
                if (is_squarefree(gcd(cache[k], idx_val)))
                    sum += cache[k];
            }
            cache[i] = sum;
        }
        cached_up_to = n;
    }

    return cache[n];
}
