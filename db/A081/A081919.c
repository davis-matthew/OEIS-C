/*
 * A081919  E.g.f.: exp(x)/sqrt(1-x^2).
 * https://oeis.org/A081919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Recursive definition a(0) = 1, a(1) = 1, a(2) = 2 for n >= 3:
 *            a(n) = a(n-1) + (n-1)^2 * a(n-2) - (n-1)*(n-2) * a(n-3)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code uses a simple memoisation cache that grows with the
   highest index requested.  Only standard library functions are used
   for allocation; no external utility headers are required.  */

#include <stdlib.h>      /* malloc, realloc */

static seq_t *cache = NULL;        /* dynamic array of computed values   */
static size_t cache_cap = 0;       /* capacity in seq_t elements         */
static idx_t  cache_max = 0;       /* highest index that has been filled */

/* Helper to allocate or grow the cache */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* start with space for the three base terms and a guard */
        cache_cap = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache_max = 2;
        return;
    }

    if ((size_t)n < cache_cap) {
        /* the needed slot already exists */
        return;
    }

    /* grow until the cache can hold index n */
    size_t new_cap = cache_cap;
    while ((size_t)n >= new_cap) {
        new_cap <<= 1;               /* double capacity */
    }
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* handle indices below the known base cases */
    if (n < 3) {
        static const seq_t base[3] = { (seq_t)1, (seq_t)1, (seq_t)2 };
        return base[n];
    }

    /* Ensure we have space up to n and compute missing entries */
    ensure_cache(n);

    /* Compute terms up to n if they haven't been computed yet */
    if (n > cache_max) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t term = cache[i - 1];                 /* a(i-1)                     */
            seq_t mul;                                 /* temporary storage          */

            /* (i-1)^2 * a(i-2) */
            mul = (seq_t)(i - 1);
            mul = mul * mul;                           /* (i-1)^2                     */
            term += mul * cache[i - 2];

            /* -(i-1)*(i-2) * a(i-3) */
            mul = (seq_t)(i - 1);
            mul = mul * (seq_t)(i - 2);                /* (i-1)*(i-2)                 */
            term -= mul * cache[i - 3];

            cache[i] = term;
        }
        cache_max = n;
    }

    return cache[n];
}
